#ifndef TCOMBOBOXDELEGATE_H
#define TCOMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

class TComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

private:
    QStringList m_itemList;
    bool m_editable;
public:
    void iniCombo(QStringList itemList,bool editable);
    explicit TComboBoxDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const ;

    void setEditorData(QWidget *editor, const QModelIndex &index) const ;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const ;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const ;
};

#endif // TCOMBOBOXDELEGATE_H
