#include "addpathmenuitem.h"
#include <QDir>

AddPathMenuItem::AddPathMenuItem(QWidget *parent)
    : MenuItem(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setIconPath(":/res/icons/common/menuitem/folder16.png");
    setText("+ Add New Path...");
    
    // Make the icon clickable too
    mIconWidget.setAttribute(Qt::WA_TransparentForMouseEvents, false);
    connect(&mIconWidget, &IconButton::clicked, this, &AddPathMenuItem::showFolderDialog);
}

void AddPathMenuItem::onPress() {
    // When user clicks anywhere on the item (or presses the shortcut number)
    showFolderDialog();
}

void AddPathMenuItem::triggerDialog() {
    // Public method to trigger the dialog from outside
    showFolderDialog();
}

void AddPathMenuItem::showFolderDialog() {
    QFileDialog dialog;
    dialog.setDirectory(QDir::homePath());
    dialog.setWindowTitle("Select directory to add");
    dialog.setWindowModality(Qt::ApplicationModal);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.setOption(QFileDialog::DontResolveSymlinks);
    
    if (dialog.exec() == QDialog::Accepted) {
        QStringList selected = dialog.selectedFiles();
        if (!selected.isEmpty()) {
            emit pathAdded(selected.first());
        }
    }
}
