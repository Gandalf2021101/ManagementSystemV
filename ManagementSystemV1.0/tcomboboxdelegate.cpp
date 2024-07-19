#include "tcomboboxdelegate.h"
#include <QComboBox>
void TComboBoxDelegate::iniCombo(QStringList itemList, bool editbale)
{
    this->m_itemList=itemList;
    this->m_editable=editbale;
}

TComboBoxDelegate::TComboBoxDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget *TComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QComboBox*editor=new QComboBox(parent);
    editor->setFrame(false);
    editor->setEditable(m_editable);
    for(int i=0;i<m_itemList.count();i++){
        editor->addItem(m_itemList.at(i));
    }
    return editor;
}

void TComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox*combobox=static_cast<QComboBox*>(editor);
    QString str=index.model()->data(index,Qt::EditRole).toString();
    combobox->setEditText(str);//??
}

void TComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox*combobox=static_cast<QComboBox*>(editor);
    QString str=combobox->currentText();
    model->setData(index,str,Qt::EditRole);

}


void TComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}




