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

#include <xlnt/workbook/name_definition.hpp>
#include <xlnt/workbook/workbook.hpp>
#include <xlnt/worksheet/worksheet.hpp>
#include <helpers/test_suite.hpp>

class name_definition_test_suite : public test_suite
{
public:
    name_definition_test_suite()
    {
        register_test(test_new_workbook_has_no_defined_names);
        register_test(test_every_defined_name_is_kept);
        register_test(test_defined_names_keep_file_order);
        register_test(test_defined_names_keep_text_as_written);
        register_test(test_defined_name_scope);
        register_test(test_defined_name_hidden);
        register_test(test_reserved_defined_names_are_kept);
    }

    // The fixture's definedNames, in file order:
    //   Simple       Data!$A$1:$A$3
    //   WholeColumn  Data!$A:$A
    //   Dynamic      OFFSET(Data!$A$1,0,0,COUNTA(Data!$A:$A),1)
    //   Constant     0.25
    //   Broken       #REF!
    //   External     [1]Prices!$B$2
    //   Scoped       Data!$A$3
    //   Secret       Data!$A$2                 hidden="1"
    //   Scoped       'Other Sheet'!$B$1        localSheetId="1"
    static std::vector<xlnt::name_definition> load_fixture()
    {
        xlnt::workbook wb;
        wb.load(path_helper::test_file("21_every_defined_name.xlsx"));
        return wb.defined_names();
    }

    static const xlnt::name_definition &find(const std::vector<xlnt::name_definition> &names,
        const std::string &name, bool scoped)
    {
        for (const auto &defined : names)
        {
            if (defined.name == name && defined.local_sheet_id.is_set() == scoped)
            {
                return defined;
            }
        }
        throw xlnt::exception("no such defined name: " + name);
    }

    void test_new_workbook_has_no_defined_names()
    {
        xlnt::workbook wb;
        xlnt_assert(wb.defined_names().empty());
    }

    void test_every_defined_name_is_kept()
    {
        // named_ranges() keeps only names that are one sheet and one valid
        // range; defined_names() keeps every one.
        xlnt_assert_equals(load_fixture().size(), 9);
    }

    void test_defined_names_keep_file_order()
    {
        const auto names = load_fixture();
        xlnt_assert_equals(names.front().name, "Simple");
        xlnt_assert_equals(names.back().name, "Scoped");
    }

    void test_defined_names_keep_text_as_written()
    {
        const auto names = load_fixture();
        xlnt_assert_equals(find(names, "Simple", false).value, "Data!$A$1:$A$3");
        xlnt_assert_equals(find(names, "WholeColumn", false).value, "Data!$A:$A");
        xlnt_assert_equals(find(names, "Dynamic", false).value,
            "OFFSET(Data!$A$1,0,0,COUNTA(Data!$A:$A),1)");
        xlnt_assert_equals(find(names, "Constant", false).value, "0.25");
        xlnt_assert_equals(find(names, "Broken", false).value, "#REF!");
        xlnt_assert_equals(find(names, "External", false).value, "[1]Prices!$B$2");
    }

    void test_defined_name_scope()
    {
        // One name defined twice: for the workbook, and for the second sheet.
        const auto names = load_fixture();
        const auto &workbook_wide = find(names, "Scoped", false);
        const auto &sheet_only = find(names, "Scoped", true);

        xlnt_assert(!workbook_wide.local_sheet_id.is_set());
        xlnt_assert_equals(workbook_wide.value, "Data!$A$3");
        xlnt_assert_equals(sheet_only.local_sheet_id.get(), 1);
        xlnt_assert_equals(sheet_only.value, "'Other Sheet'!$B$1");
    }

    void test_defined_name_hidden()
    {
        const auto names = load_fixture();
        xlnt_assert(find(names, "Secret", false).hidden);
        xlnt_assert(!find(names, "Simple", false).hidden);
    }

    void test_reserved_defined_names_are_kept()
    {
        // The _xlnm. names still configure the worksheet, and are listed too.
        xlnt::workbook wb;
        wb.load(path_helper::test_file("19_defined_names.xlsx"));
        bool reserved = false;
        for (const auto &defined : wb.defined_names())
        {
            reserved = reserved || defined.name.rfind("_xlnm.", 0) == 0;
        }
        xlnt_assert(reserved);
        xlnt_assert(wb.sheet_by_index(1).has_print_area());
    }
};

static name_definition_test_suite x;
