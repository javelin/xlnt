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

#include <xlnt/cell/cell.hpp>
#include <xlnt/workbook/workbook.hpp>
#include <xlnt/worksheet/range_reference.hpp>
#include <xlnt/worksheet/worksheet.hpp>
#include <helpers/test_suite.hpp>

class cell_metadata_test_suite : public test_suite
{
public:
    cell_metadata_test_suite()
    {
        register_test(test_dynamic_array_keeps_its_metadata_index);
        register_test(test_legacy_array_formula_has_no_metadata);
        register_test(test_plain_formula_has_no_metadata);
        register_test(test_value_cell_has_no_metadata);
    }

    // The fixture's first sheet:
    //   A1:A3  1, 2, 3
    //   B1     cm="1", an array formula over B1:B3 - Excel's dynamic array
    //   C1     a legacy single-cell array formula, no cm
    //   D1     an ordinary formula
    static xlnt::worksheet load_fixture(xlnt::workbook &wb)
    {
        wb.load(path_helper::test_file("27_cell_metadata.xlsx"));
        return wb.active_sheet();
    }

    void test_dynamic_array_keeps_its_metadata_index()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        const auto cell = ws.cell("B1");
        xlnt_assert(cell.has_array_formula());
        xlnt_assert_equals(cell.array_formula_range(), xlnt::range_reference("B1:B3"));
        xlnt_assert(cell.has_cell_metadata());
        xlnt_assert_equals(cell.cell_metadata_index(), 1u);
    }

    void test_legacy_array_formula_has_no_metadata()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert(ws.cell("C1").has_array_formula());
        xlnt_assert(!ws.cell("C1").has_cell_metadata());
    }

    void test_plain_formula_has_no_metadata()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert(!ws.cell("D1").has_cell_metadata());
    }

    void test_value_cell_has_no_metadata()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert(!ws.cell("A1").has_cell_metadata());
    }
};

static cell_metadata_test_suite x;
