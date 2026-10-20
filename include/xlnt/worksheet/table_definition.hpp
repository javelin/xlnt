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


#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include <xlnt/xlnt_config.hpp>
#include <xlnt/worksheet/range_reference.hpp>

namespace xlnt {

/// <summary>
/// A table (ListObject) defined on a worksheet, as read from its table part.
/// This is what a structured reference such as Table1[Amount] names: the
/// formula text carries only the table and column names, so evaluating one
/// needs the range and column order recorded here.
/// </summary>
struct XLNT_API table_definition
{
    /// <summary>
    /// The name formulas use (the table part's displayName, or its name when
    /// displayName is absent).
    /// </summary>
    std::string name;

    /// <summary>
    /// The whole table, header and totals rows included.
    /// </summary>
    range_reference ref;

    /// <summary>
    /// Rows of headers at the top of ref: 1 by default, 0 for a table whose
    /// header row is hidden.
    /// </summary>
    std::size_t header_row_count = 1;

    /// <summary>
    /// Rows of totals at the bottom of ref: 0 unless a totals row is shown.
    /// </summary>
    std::size_t totals_row_count = 0;

    /// <summary>
    /// Column names, left to right.
    /// </summary>
    std::vector<std::string> columns;

    bool operator==(const table_definition &other) const
    {
        return name == other.name && ref == other.ref
            && header_row_count == other.header_row_count
            && totals_row_count == other.totals_row_count && columns == other.columns;
    }

    bool operator!=(const table_definition &other) const
    {
        return !(*this == other);
    }
};

} // namespace xlnt
