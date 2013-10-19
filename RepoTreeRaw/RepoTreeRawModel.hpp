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

#ifndef MGV_REPO_TREE_RAW_MODEL_HPP
#define MGV_REPO_TREE_RAW_MODEL_HPP

#include <QAbstractItemModel>

namespace RM {
    class Repo;
    class Base;
}

class RepoTreeRawModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    RepoTreeRawModel(QObject* parent = 0);

public:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const ;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private slots:
    void objectCreated(RM::Repo* repo, RM::Base* object);
    void objectAboutToBeRemoved(RM::Repo* repo, RM::Base* object);

private:
    class Node;

    Node* index2node(const QModelIndex& index) const;
    QModelIndex node2Index(Node* node, int col = 0) const;

    Node* root;
};

#endif
