
#include <QPixmap>
#include <QIcon>

#include "libHeaven/Icons/Icon.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"

#include "RepoTreeRawModel.hpp"

class RepoTreeRawModel::Node
{
public:
    typedef QVector< Node* > List;

public:
    Node(Node* _parent, RM::Base* _object);
    ~Node();

public:
    Node* parent() const;
    RM::Base* object() const;
    List children() const;
    int removeChild(RM::Base* object);
    int insertChild(RM::Base* object);

    QVariant data(int column, int role) const;

    void scanChildren();

    Node* findNode(RM::Base* object);

private:
    Node*       mParent;
    RM::Base*   mObject;
    List        mChildren;
};

RepoTreeRawModel::Node::Node(RepoTreeRawModel::Node *_parent, RM::Base *_object)
    : mParent(_parent)
    , mObject(_object)
{
    scanChildren();
}

RepoTreeRawModel::Node::~Node()
{
    // Due to constraints OF RepoMan, we should be sure that we can only be deleted when we have
    // no children.
    Q_ASSERT(mChildren.count() == 0);
    mParent->removeChild(mObject);
}

RepoTreeRawModel::Node* RepoTreeRawModel::Node::parent() const
{
    return mParent;
}

RM::Base* RepoTreeRawModel::Node::object() const
{
    return mObject;
}

RepoTreeRawModel::Node::List RepoTreeRawModel::Node::children() const
{
    return mChildren;
}

int RepoTreeRawModel::Node::removeChild(RM::Base* object)
{
    return 0;
}

int RepoTreeRawModel::Node::insertChild(RM::Base* object)
{
    Node* node = new Node(this, object);
    QString name = object->displayName();

    if (mChildren.count() == 0) {
        mChildren.append(node);
        return 0;
    }

    for (int i = 0; i < mChildren.count(); i++) {
        Node* old = mChildren.at(i);

        if (old->object()->displayName() > name) {
            mChildren.insert(i, node);
            return i;
        }
    }

    mChildren.append(node);
    return mChildren.count();
}

QVariant RepoTreeRawModel::Node::data(int column, int role) const
{
    if (role == Qt::DisplayRole) {
        if (column == 0) {
            return mObject->displayName();
        }
        else {
            return mObject->typeName();
        }
    }

    if (role == Qt::DecorationRole) {
        if (column == 0) {
            return QIcon(mObject->icon().icon().pixmap());
        }
    }

    return QVariant();
}

void RepoTreeRawModel::Node::scanChildren()
{
    foreach (RM::Base* obj, mObject->childObjects())
    {
        insertChild(obj);
    }
}

RepoTreeRawModel::Node* RepoTreeRawModel::Node::findNode(RM::Base* object)
{
    if (object == mObject) {
        return this;
    }

    foreach (Node* subNode, mChildren) {
        Node* found = subNode->findNode(object);
        if (found) {
            return found;
        }
    }

    return NULL;
}

//----------------------------------------------------------------------------------------------- 8<

RepoTreeRawModel::RepoTreeRawModel(QObject* parent)
    : QAbstractItemModel(parent)
{
    root = new Node(NULL, &MacGitver::repoMan());
}

QModelIndex RepoTreeRawModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(column);
    Node::List list;

    if (parent.isValid()) {

        Node* nodeParent = index2node(parent);
        if(!nodeParent) {
            return QModelIndex();
        }
        list = nodeParent->children();
    }
    else
    {
        list = root->children();
    }

    if (row >= list.count() || row < 0) {
        return QModelIndex();
    }

    return node2Index(list.at(row), column);
}

QModelIndex RepoTreeRawModel::parent(const QModelIndex& child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    Node* node = index2node(child);
    if (!node || !node->parent()) {
        return QModelIndex();
    }

    return node2Index(node->parent());
}

int RepoTreeRawModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        Node* node = index2node(parent);
        Q_ASSERT(node);
        return node->children().count();
    }

    return root->children().count();
}

int RepoTreeRawModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant RepoTreeRawModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid()) {
        Node* node = index2node(index);
        Q_ASSERT(node);
        return node->data(index.column(), role);
    }
    return QVariant();
}


RepoTreeRawModel::Node* RepoTreeRawModel::index2node(const QModelIndex& index) const
{
    return static_cast<RepoTreeRawModel::Node*>(index.internalPointer());
}

QModelIndex RepoTreeRawModel::node2Index(RepoTreeRawModel::Node* node, int col) const
{
    int row = 0;

    if (!node) {
        return QModelIndex();
    }

    Node::List list;

    if (node->parent()) {
        list = node->parent()->children();
    }
    else {
        list = root->children();
    }

    row = list.indexOf(node);
    if (row == -1) {
        return QModelIndex();
    }

    return createIndex(row, col,  node);
}

void RepoTreeRawModel::objectCreated(RM::Repo* repo, RM::Base* object)
{
    Q_UNUSED(repo);
    RM::Base* parentObject = object->parentObject();
    Node* parentNode = root->findNode(parentObject);
    if (!parentNode) {
        return;
    }

    int i = parentNode->insertChild(object);
}

void RepoTreeRawModel::objectAboutToBeRemoved(RM::Repo* repo, RM::Base* object)
{
    Q_UNUSED(repo);
}
