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

class array_formula_test_suite : public test_suite
{
public:
    array_formula_test_suite()
    {
        register_test(test_every_cell_of_a_block_knows_the_block);
        register_test(test_single_cell_array_formula);
        register_test(test_ordinary_formula_is_not_an_array);
        register_test(test_cell_without_formula_is_not_an_array);
        register_test(test_new_formula_is_not_an_array);
    }

    // The fixture's first sheet:
    //   A1:A3  1, 2, 3
    //   B1:B3  one array formula, {=A1:A3*10}
    //   D1     a single-cell array formula, {=SUM(A1:A3*2)}
    //   E1     =A1+1, an ordinary formula
    static xlnt::worksheet load_fixture(xlnt::workbook &wb)
    {
        wb.load(path_helper::test_file("23_array_formulae.xlsx"));
        return wb.active_sheet();
    }

    void test_every_cell_of_a_block_knows_the_block()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        for (const auto *reference : {"B1", "B2", "B3"})
        {
            xlnt_assert(ws.cell(reference).has_array_formula());
            xlnt_assert_equals(ws.cell(reference).array_formula_range(), xlnt::range_reference("B1:B3"));
        }
    }

    void test_single_cell_array_formula()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert(ws.cell("D1").has_array_formula());
        xlnt_assert_equals(ws.cell("D1").array_formula_range(), xlnt::range_reference("D1:D1"));
    }

    void test_ordinary_formula_is_not_an_array()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert(ws.cell("E1").has_formula());
        xlnt_assert(!ws.cell("E1").has_array_formula());
    }

    void test_cell_without_formula_is_not_an_array()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert(!ws.cell("A1").has_array_formula());
    }

    void test_new_formula_is_not_an_array()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        ws.cell("A1").formula("=1+1");
        xlnt_assert(!ws.cell("A1").has_array_formula());
    }
};

static array_formula_test_suite x;
