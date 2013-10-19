/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
 * (C) Cunz RaD Ltd.
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MGV_VIEW_REPOTREERAW_HPP
#define MGV_VIEW_REPOTREERAW_HPP

#include "libHeaven/CentralUI/Views/View.hpp"

class QTreeView;

class RepoTreeRawView : public Heaven::View
{
    Q_OBJECT
public:
    RepoTreeRawView();

private:
    void setupUi();

private:
    QTreeView*      mTree;
};

#endif
