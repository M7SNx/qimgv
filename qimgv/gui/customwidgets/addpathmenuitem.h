#pragma once

#include <QFileDialog>
#include "gui/customwidgets/menuitem.h"

class AddPathMenuItem : public MenuItem {
    Q_OBJECT
public:
    AddPathMenuItem(QWidget *parent);
    void triggerDialog();  // Public method to trigger the dialog

protected:
    virtual void onPress() override;

signals:
    void pathAdded(QString);

private:
    void showFolderDialog();
};
