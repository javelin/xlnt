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

#include <xlnt/xlnt_config.hpp>
#include <xlnt/utils/optional.hpp>

namespace xlnt {

/// <summary>
/// A defined name exactly as the workbook part writes it. named_range keeps
/// only names whose text is one sheet and one valid range; this keeps every
/// name - a formula such as OFFSET(Sheet1!$A$1,0,0,COUNTA(Sheet1!$A:$A),1),
/// a constant, a whole column, a reference Excel has marked #REF!, a name
/// referring to another workbook - so a consumer can evaluate them.
/// </summary>
struct XLNT_API name_definition
{
    /// <summary>
    /// The name formulas use, including Excel's reserved _xlnm. prefix for
    /// the built-in ones such as _xlnm.Print_Area.
    /// </summary>
    std::string name;

    /// <summary>
    /// The definition as written in the file, without a leading '='.
    /// </summary>
    std::string value;

    /// <summary>
    /// The zero-based sheet index a sheet-scoped name belongs to; unset for a
    /// name scoped to the whole workbook.
    /// </summary>
    optional<std::size_t> local_sheet_id;

    /// <summary>
    /// True for a name hidden from Excel's name manager.
    /// </summary>
    bool hidden = false;

    bool operator==(const name_definition &other) const
    {
        return name == other.name && value == other.value
            && local_sheet_id == other.local_sheet_id && hidden == other.hidden;
    }

    bool operator!=(const name_definition &other) const
    {
        return !(*this == other);
    }
};

} // namespace xlnt
