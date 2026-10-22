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
#include <xlnt/cell/cell_reference.hpp>
#include <xlnt/workbook/workbook.hpp>
#include <xlnt/worksheet/worksheet.hpp>
#include <helpers/test_suite.hpp>

class shared_formula_test_suite : public test_suite
{
public:
    shared_formula_test_suite()
    {
        register_test(test_shared_formula_member_knows_its_group);
        register_test(test_shared_formula_master_is_its_own_master);
        register_test(test_second_group_has_its_own_master);
        register_test(test_member_formula_is_the_masters_text);
        register_test(test_ordinary_formula_is_not_shared);
        register_test(test_cell_without_formula_is_not_shared);
        register_test(test_new_formula_is_not_shared);
    }

    // The fixture's first sheet:
    //   A1:A3  1, 2, 3
    //   B1:B3  shared group 0, master B1: A1*2
    //   C1     =A1+100, an ordinary formula
    //   D2:D3  shared group 1, master D2: A2+B2
    static xlnt::worksheet load_fixture(xlnt::workbook &wb)
    {
        wb.load(path_helper::test_file("22_shared_formulae.xlsx"));
        return wb.active_sheet();
    }

    void test_shared_formula_member_knows_its_group()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert(ws.cell("B2").has_shared_formula());
        xlnt_assert(ws.cell("B3").has_shared_formula());
        xlnt_assert_equals(ws.cell("B2").shared_formula_master(), xlnt::cell_reference("B1"));
        xlnt_assert_equals(ws.cell("B3").shared_formula_master(), xlnt::cell_reference("B1"));
    }

    void test_shared_formula_master_is_its_own_master()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert(ws.cell("B1").has_shared_formula());
        xlnt_assert_equals(ws.cell("B1").shared_formula_master(), xlnt::cell_reference("B1"));
    }

    void test_second_group_has_its_own_master()
    {
        // Group 1 starts below row 1, so the master is not found by position.
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert_equals(ws.cell("D2").shared_formula_master(), xlnt::cell_reference("D2"));
        xlnt_assert_equals(ws.cell("D3").shared_formula_master(), xlnt::cell_reference("D2"));
    }

    void test_member_formula_is_the_masters_text()
    {
        // A member reports the master's text un-anchored: re-anchoring it by
        // the offset from shared_formula_master() is the caller's job.
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert_equals(ws.cell("B1").formula(), "A1*2");
        xlnt_assert_equals(ws.cell("B3").formula(), "A1*2");
        xlnt_assert_equals(ws.cell("D3").formula(), "A2+B2");
    }

    void test_ordinary_formula_is_not_shared()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert(ws.cell("C1").has_formula());
        xlnt_assert(!ws.cell("C1").has_shared_formula());
    }

    void test_cell_without_formula_is_not_shared()
    {
        xlnt::workbook wb;
        auto ws = load_fixture(wb);
        xlnt_assert(!ws.cell("A1").has_shared_formula());
    }

    void test_new_formula_is_not_shared()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        ws.cell("A1").formula("=1+1");
        xlnt_assert(!ws.cell("A1").has_shared_formula());
    }
};

static shared_formula_test_suite x;
