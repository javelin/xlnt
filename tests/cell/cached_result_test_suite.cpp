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
#include <xlnt/worksheet/worksheet.hpp>
#include <helpers/test_suite.hpp>

class cached_result_test_suite : public test_suite
{
public:
    cached_result_test_suite()
    {
        register_test(test_cached_empty_text_is_a_value);
        register_test(test_self_closing_empty_text_is_a_value);
        register_test(test_formula_never_calculated_has_no_value);
        register_test(test_text_formula_without_value_has_no_value);
        register_test(test_cached_text_and_number_are_unchanged);
    }

    // The fixture's first sheet - every formula cell differs only in what it
    // caches:
    //   B1  t="str", <v></v>   Excel's way of storing a formula answering ""
    //   B4  t="str", <v/>      the same, self-closing
    //   B3  no type, <v />     openpyxl's way of storing a formula it never calculated
    //   B5  t="str", no <v>    a type with no value to go with it
    //   B2  t="str", <v>x</v>  and C1 <v>2</v>, ordinary cached answers
    static xlnt::worksheet load_fixture(xlnt::workbook &wb)
    {
        wb.load(path_helper::test_file("26_cached_results.xlsx"));
        return wb.active_sheet();
    }

    void test_cached_empty_text_is_a_value()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        const auto cell = ws.cell("B1");
        xlnt_assert(cell.has_formula());
        xlnt_assert(cell.has_value());
        xlnt_assert_equals(cell.data_type(), xlnt::cell::type::formula_string);
        xlnt_assert_equals(cell.value<std::string>(), "");
    }

    void test_self_closing_empty_text_is_a_value()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        const auto cell = ws.cell("B4");
        xlnt_assert(cell.has_value());
        xlnt_assert_equals(cell.data_type(), xlnt::cell::type::formula_string);
        xlnt_assert_equals(cell.value<std::string>(), "");
    }

    void test_formula_never_calculated_has_no_value()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        const auto cell = ws.cell("B3");
        xlnt_assert(cell.has_formula());
        xlnt_assert(!cell.has_value());
        xlnt_assert_equals(cell.data_type(), xlnt::cell::type::empty);
    }

    void test_text_formula_without_value_has_no_value()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert(ws.cell("B5").has_formula());
        xlnt_assert(!ws.cell("B5").has_value());
    }

    void test_cached_text_and_number_are_unchanged()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert_equals(ws.cell("B2").data_type(), xlnt::cell::type::formula_string);
        xlnt_assert_equals(ws.cell("B2").value<std::string>(), "x");
        xlnt_assert_equals(ws.cell("C1").value<double>(), 2.0);
    }
};

static cached_result_test_suite x;
