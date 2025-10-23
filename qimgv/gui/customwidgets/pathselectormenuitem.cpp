#include "pathselectormenuitem.h"

PathSelectorMenuItem::PathSelectorMenuItem(QWidget *parent)
    : MenuItem(parent),
      mDirectory(""),
      mPath("")
{
    setFocusPolicy(Qt::NoFocus);
    setIconPath(":/res/icons/common/menuitem/folder16.png");
    mIconWidget.setAttribute(Qt::WA_TransparentForMouseEvents, false);
    connect(&mIconWidget, &IconButton::clicked, [this]() {
        QFileDialog dialog;
        dialog.setDirectory(mDirectory);
        dialog.setWindowTitle("Select directory");
        dialog.setWindowModality(Qt::ApplicationModal);
        dialog.setFileMode(QFileDialog::Directory);
        dialog.setOption(QFileDialog::ShowDirsOnly);
        dialog.setOption(QFileDialog::DontResolveSymlinks);
        connect(&dialog, &QFileDialog::fileSelected, this, &PathSelectorMenuItem::setDirectory);
        dialog.exec();
    });
}

QString PathSelectorMenuItem::directory() {
    return mDirectory;
}

QString PathSelectorMenuItem::path() {
    return mPath;
}

void PathSelectorMenuItem::setDirectory(QString path) {
    mDirectory = path;
    mPath = path;
    QString stripped;
    
    // Handle empty paths safely
    if (path.isEmpty()) {
        stripped = "";
    } else {
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
        QStringList parts = mDirectory.split("/");
        stripped = parts.isEmpty() ? "" : parts.last();
#else
        QStringList parts = mDirectory.split("/", Qt::SkipEmptyParts);
        stripped = parts.isEmpty() ? "" : parts.last();
#endif
    }
    
    this->mTextLabel.setText(stripped);
}

void PathSelectorMenuItem::onPress() {
    if(!mDirectory.isEmpty())
        emit directorySelected(mDirectory);
}
