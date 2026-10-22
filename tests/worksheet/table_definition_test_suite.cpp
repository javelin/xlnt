// Copyright (c) 2014-2022 Thomas Fussell
// Copyright (c) 2024-2026 xlnt-community
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE
//
// @license: http://www.opensource.org/licenses/mit-license.php
// @author: see AUTHORS file

#include <string>
#include <vector>

#include <xlnt/cell/cell.hpp>
#include <xlnt/workbook/workbook.hpp>
#include <xlnt/worksheet/table_definition.hpp>
#include <xlnt/worksheet/worksheet.hpp>
#include <helpers/test_suite.hpp>

class table_definition_test_suite : public test_suite
{
public:
    table_definition_test_suite()
    {
        register_test(test_new_worksheet_has_no_tables);
        register_test(test_every_readable_table_is_read);
        register_test(test_table_with_totals_row);
        register_test(test_table_without_header_row);
        register_test(test_package_absolute_target_is_read);
        register_test(test_malformed_table_part_is_skipped);
    }

    // The fixture:
    //   First   Sales  A1:B4  header 1, totals 1, columns Region, Units
    //           Costs  E1:F2  header 0, totals 0, columns Low, High
    //           a third relationship to a part that is not XML at all
    //   Second  Remote A1:B2  related by the package-absolute target
    //                         /xl/tables/table3.xml
    static xlnt::table_definition find(const xlnt::worksheet &ws, const std::string &name)
    {
        for (const auto &table : ws.tables())
        {
            if (table.name == name)
            {
                return table;
            }
        }
        throw xlnt::exception("no such table: " + name);
    }

    static xlnt::workbook load_fixture()
    {
        xlnt::workbook wb;
        wb.load(path_helper::test_file("25_table_parts.xlsx"));
        return wb;
    }

    void test_new_worksheet_has_no_tables()
    {
        xlnt::workbook wb;
        xlnt_assert(wb.active_sheet().tables().empty());
    }

    void test_every_readable_table_is_read()
    {
        // In no particular order: the package keeps relationships unordered.
        const auto tables = load_fixture().sheet_by_title("First").tables();
        xlnt_assert_equals(tables.size(), 2);
        xlnt_assert_differs(tables[0].name, tables[1].name);
    }

    void test_table_with_totals_row()
    {
        const auto sales = find(load_fixture().sheet_by_title("First"), "Sales");
        xlnt_assert_equals(sales.ref, xlnt::range_reference("A1:B4"));
        xlnt_assert_equals(sales.header_row_count, 1);
        xlnt_assert_equals(sales.totals_row_count, 1);
        xlnt_assert_equals(sales.columns, std::vector<std::string>({"Region", "Units"}));
    }

    void test_table_without_header_row()
    {
        const auto costs = find(load_fixture().sheet_by_title("First"), "Costs");
        xlnt_assert_equals(costs.ref, xlnt::range_reference("E1:F2"));
        xlnt_assert_equals(costs.header_row_count, 0);
        xlnt_assert_equals(costs.totals_row_count, 0);
        xlnt_assert_equals(costs.columns, std::vector<std::string>({"Low", "High"}));
    }

    void test_package_absolute_target_is_read()
    {
        // The target arrives relative to the wrong folder; the reader falls
        // back to the ancestor folder that holds the part.
        const auto wb = load_fixture();
        const auto second = wb.sheet_by_title("Second");
        xlnt_assert_equals(second.tables().size(), 1);
        xlnt_assert_equals(find(second, "Remote").ref, xlnt::range_reference("A1:B2"));
    }

    void test_malformed_table_part_is_skipped()
    {
        // The broken part costs its own table and nothing else.
        auto wb = load_fixture();
        xlnt_assert_equals(wb.sheet_by_title("First").cell("B2").value<int>(), 10);
        xlnt_assert_equals(wb.sheet_by_title("First").tables().size(), 2);
    }
};

static table_definition_test_suite x;
