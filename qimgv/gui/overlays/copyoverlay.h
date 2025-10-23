#pragma once

#include "gui/customwidgets/overlaywidget.h"
#include <QMap>

class QKeyEvent;
class QWheelEvent;
class FloatingWidgetContainer;
class PathSelectorMenuItem;
class AddPathMenuItem;

namespace Ui {
    class CopyOverlay;
}

enum CopyOverlayMode {
    OVERLAY_COPY,
    OVERLAY_MOVE
};

class CopyOverlay : public OverlayWidget {
    Q_OBJECT
public:
    CopyOverlay(FloatingWidgetContainer *parent);
    ~CopyOverlay();
    void saveSettings();
    void setDialogMode(CopyOverlayMode _mode);
    CopyOverlayMode operationMode();

public slots:
    void show();
    void hide();

signals:
    void copyRequested(QString);
    void moveRequested(QString);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    bool focusNextPrevChild(bool) override;

private slots:
    void requestFileOperation(QString path);
    void readSettings();
    void nextPage();
    void prevPage();
    void addNewPath(QString path);

private:
    void createDefaultPaths();
    void createPathWidgets();
    void createShortcuts();
    void removePathWidgets();
    void updatePageUI();

    Ui::CopyOverlay *ui;
    QList<PathSelectorMenuItem*> pathWidgets;
    QList<AddPathMenuItem*> addPathWidgets;  // Track "Add New Path" items
    QStringList paths;
    QMap<QString, int> shortcuts;
    CopyOverlayMode mode;

    int currentPage;
    const int itemsPerPage = 9;
    bool pathWidgetsCreated;
};
