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

#include <cstdint>
#include <vector>

#include <xlnt/workbook/calculation_properties.hpp>
#include <xlnt/workbook/workbook.hpp>
#include <helpers/test_suite.hpp>

class calculation_properties_test_suite : public test_suite
{
public:
    calculation_properties_test_suite()
    {
        register_test(test_iteration_settings_are_read);
        register_test(test_iteration_defaults);
        register_test(test_iteration_settings_round_trip);
        register_test(test_iteration_off_round_trips_as_off);
    }

    // The fixture's calcPr: iterate="1" iterateCount="50" iterateDelta="0.01".
    void test_iteration_settings_are_read()
    {
        xlnt::workbook wb;
        wb.load(path_helper::test_file("24_iterative_calculation.xlsx"));
        xlnt_assert(wb.has_calculation_properties());
        const auto props = wb.calculation_properties();
        xlnt_assert(props.iterate);
        xlnt_assert_equals(props.iterate_count, 50);
        xlnt_assert_delta(props.iterate_delta, 0.01, 1e-12);
    }

    void test_iteration_defaults()
    {
        // Excel's defaults, when calcPr says nothing about iteration.
        const xlnt::calculation_properties props;
        xlnt_assert(!props.iterate);
        xlnt_assert_equals(props.iterate_count, 100);
        xlnt_assert_delta(props.iterate_delta, 0.001, 1e-12);
    }

    void test_iteration_settings_round_trip()
    {
        xlnt::workbook wb;
        xlnt::calculation_properties props;
        props.iterate = true;
        props.iterate_count = 7;
        props.iterate_delta = 0.25;
        wb.calculation_properties(props);

        std::vector<std::uint8_t> data;
        wb.save(data);
        xlnt::workbook reloaded;
        reloaded.load(data);

        const auto read = reloaded.calculation_properties();
        xlnt_assert(read.iterate);
        xlnt_assert_equals(read.iterate_count, 7);
        xlnt_assert_delta(read.iterate_delta, 0.25, 1e-12);
    }

    void test_iteration_off_round_trips_as_off()
    {
        // Iteration off is not written at all, and reads back as the default.
        xlnt::workbook wb;
        wb.calculation_properties(xlnt::calculation_properties());

        std::vector<std::uint8_t> data;
        wb.save(data);
        xlnt::workbook reloaded;
        reloaded.load(data);

        xlnt_assert(!reloaded.calculation_properties().iterate);
        xlnt_assert_equals(reloaded.calculation_properties().iterate_count, 100);
    }
};

static calculation_properties_test_suite x;
