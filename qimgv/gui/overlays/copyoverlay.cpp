#include "copyoverlay.h"
#include "ui_copyoverlay.h"
#include "settings.h"
#include "gui/customwidgets/pathselectormenuitem.h"
#include "gui/customwidgets/addpathmenuitem.h"
#include "gui/customwidgets/floatingwidgetcontainer.h"
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QDir>
#include <QFileInfo>
#include <QCursor>
#include <QTimer>
#include <cmath>
#include <algorithm>

CopyOverlay::CopyOverlay(FloatingWidgetContainer *parent) :
    OverlayWidget(parent),
    ui(new Ui::CopyOverlay),
    currentPage(0),
    pathWidgetsCreated(false)
{
    ui->setupUi(this);
    hide();
    setFadeEnabled(true);

    ui->closeButton->setIconPath(":/res/icons/common/overlay/close-dim16.png");
    ui->headerIcon->setIconPath(":/res/icons/common/overlay/copy16.png");
    ui->headerLabel->setText(tr("Copy to..."));
    mode = OVERLAY_COPY;

    ui->prevButton->setIconPath(":/res/icons/common/overlay/arrow_left_20.png");
    ui->nextButton->setIconPath(":/res/icons/common/overlay/arrow_right_20.png");

    createShortcuts();

    // Load paths and create widgets immediately to ensure proper sizing
    paths = settings->savedPaths();
    if (paths.count() == 0 || (paths.count() == 1 && (paths.at(0).isEmpty() || paths.at(0)[0] == '@'))) {
        paths.clear();
        paths << QDir::homePath();
        createDefaultPaths();
    }
    createPathWidgets();
    pathWidgetsCreated = true;

    setAcceptKeyboardFocus(true);

    // Force layout to process before setting container size
    // This ensures sizeHint() returns the correct size
    if (parent) {
        adjustSize();
        updateGeometry();
        setContainerSize(parent->size());
    }

    readSettings();
    connect(settings, &Settings::settingsChanged, this, &CopyOverlay::readSettings);
    connect(ui->prevButton, &IconButton::clicked, this, &CopyOverlay::prevPage);
    connect(ui->nextButton, &IconButton::clicked, this, &CopyOverlay::nextPage);
}

CopyOverlay::~CopyOverlay() {
    delete ui;
}

void CopyOverlay::show() {
    // Reload paths from settings to check if they changed
    QStringList newPaths = settings->savedPaths();
    
    // Check if we need to create defaults
    if (newPaths.count() == 0 || (newPaths.count() == 1 && (newPaths.at(0).isEmpty() || newPaths.at(0)[0] == '@'))) {
        newPaths.clear();
        newPaths << QDir::homePath();
    }
    
    // Only recreate widgets if paths have actually changed
    if (newPaths != paths) {
        paths = newPaths;
        if (paths.count() == 0 || (paths.count() == 1 && (paths.at(0).isEmpty() || paths.at(0)[0] == '@'))) {
            paths.clear();
            paths << QDir::homePath();
            createDefaultPaths();
        }
        createPathWidgets();
        pathWidgetsCreated = true;
    }

    currentPage = 0;
    updatePageUI();
    OverlayWidget::show();
    setFocus();
}

void CopyOverlay::hide() {
    OverlayWidget::hide();
    
    // If we're marked for recreation, clean up now while overlay is hidden
    if (!pathWidgetsCreated) {
        removePathWidgets();
    }
}

void CopyOverlay::removePathWidgets() {
    // Disconnect all signals from path widgets first to prevent any dangling connections
    for (PathSelectorMenuItem* item : pathWidgets) {
        if (item) {
            disconnect(item, nullptr, this, nullptr);
        }
    }
    pathWidgets.clear();
    
    // Disconnect and clear add path widgets
    for (AddPathMenuItem* item : addPathWidgets) {
        if (item) {
            disconnect(item, nullptr, this, nullptr);
        }
    }
    addPathWidgets.clear();
    
    // Remove and delete all page widgets from the stacked widget
    while(ui->stackedWidget->count() > 0) {
        QWidget* pageWidget = ui->stackedWidget->widget(0);
        ui->stackedWidget->removeWidget(pageWidget);
        // Use deleteLater to avoid issues with pending events
        pageWidget->deleteLater();
    }
}

void CopyOverlay::createPathWidgets() {
    removePathWidgets(); // Start with a clean slate
    
    int pathCount = paths.length();
    
    // Calculate pages - always show at least one page
    // Add 1 slot for "Add New..." button
    int totalSlots = pathCount + 1;
    int pageCount = std::max(1, (int)std::ceil(static_cast<double>(totalSlots) / itemsPerPage));

    for (int page = 0; page < pageCount; ++page) {
        QWidget *pageWidget = new QWidget();
        QVBoxLayout *pageLayout = new QVBoxLayout(pageWidget);
        pageLayout->setContentsMargins(0,0,0,0);
        pageLayout->setSpacing(0);
        
        int start = page * itemsPerPage;
        int end = std::min(start + itemsPerPage, pathCount);

        // Add existing paths
        for (int i = start; i < end; ++i) {
            PathSelectorMenuItem *item = new PathSelectorMenuItem(pageWidget);
            item->setDirectory(paths.at(i));
            // Set shortcut text for items on this page (1-9 for each page)
            int positionOnPage = i - start;
            if (positionOnPage < itemsPerPage) {
                item->setShortcutText(QString::number(positionOnPage + 1));
            }
            connect(item, &PathSelectorMenuItem::directorySelected, this, &CopyOverlay::requestFileOperation);
            pathWidgets.append(item);
            pageLayout->addWidget(item);
        }
        
        // Add "Add New Path..." item if there's room on this page
        if (end == pathCount && (end - start) < itemsPerPage) {
            AddPathMenuItem *addNewItem = new AddPathMenuItem(pageWidget);
            int positionOnPage = end - start;
            if (positionOnPage < itemsPerPage) {
                addNewItem->setShortcutText(QString::number(positionOnPage + 1));
            }
            // Connect to add new path handler
            connect(addNewItem, &AddPathMenuItem::pathAdded, this, &CopyOverlay::addNewPath);
            addPathWidgets.append(addNewItem);
            pageLayout->addWidget(addNewItem);
        }
        
        pageLayout->addStretch();
        ui->stackedWidget->addWidget(pageWidget);
    }
}

void CopyOverlay::updatePageUI() {
    int count = ui->stackedWidget->count();
    
    ui->footerWidget->setVisible(count > 1);
    
    if (count > 0) {
        ui->stackedWidget->setCurrentIndex(currentPage);
        ui->pageIndicator->setText(QString("(%1 / %2)").arg(currentPage + 1).arg(count));
        ui->prevButton->setEnabled(currentPage > 0);
        ui->nextButton->setEnabled(currentPage < count - 1);
    } else {
        ui->pageIndicator->setText("(0 / 0)");
        ui->prevButton->setEnabled(false);
        ui->nextButton->setEnabled(false);
    }
}

void CopyOverlay::nextPage() {
    if (currentPage < ui->stackedWidget->count() - 1) {
        currentPage++;
        updatePageUI();
    }
}

void CopyOverlay::prevPage() {
    if (currentPage > 0) {
        currentPage--;
        updatePageUI();
    }
}

void CopyOverlay::wheelEvent(QWheelEvent *event) {
    // Check if mouse is over the overlay widget itself (not parent viewer)
    QPoint localPos = this->mapFromGlobal(QCursor::pos());
    if (this->rect().contains(localPos)) {
        // Mouse is over overlay - use for page navigation
        if (event->angleDelta().y() < 0) {
            nextPage();
        } else if (event->angleDelta().y() > 0) {
            prevPage();
        }
        event->accept();
    } else {
        // Mouse is outside overlay - let parent handle it (image navigation)
        event->ignore();
    }
}

void CopyOverlay::setDialogMode(CopyOverlayMode _mode) {
    mode = _mode;
    if (mode == OVERLAY_COPY) {
        ui->headerIcon->setIconPath(":/res/icons/common/overlay/copy16.png");
        ui->headerLabel->setText(tr("Copy to..."));
    } else {
        ui->headerIcon->setIconPath(":/res/icons/common/overlay/move16.png");
        ui->headerLabel->setText(tr("Move to..."));
    }
}

CopyOverlayMode CopyOverlay::operationMode() {
    return mode;
}

void CopyOverlay::createShortcuts() {
    shortcuts.clear();
    for (int i = 0; i < itemsPerPage; i++)
        shortcuts.insert(QString::number(i + 1), i);
}

void CopyOverlay::requestFileOperation(QString path) {
    if (mode == OVERLAY_COPY)
        emit copyRequested(path);
    else
        emit moveRequested(path);
}

void CopyOverlay::addNewPath(QString path) {
    if (path.isEmpty()) {
        return; // User cancelled the dialog
    }
    
    // Add the new path to our list
    if (!paths.contains(path)) {
        paths.append(path);
        
        // Save to settings immediately
        settings->setSavedPaths(paths);
        
        // Recreate widgets to show the new path
        createPathWidgets();
        
        // Calculate which page the new path is on
        // New path is at the end (paths.count() - 1)
        int newPathIndex = paths.count() - 1;
        int newPathPage = newPathIndex / itemsPerPage;
        
        // Navigate to the page containing the new path
        currentPage = newPathPage;
        updatePageUI();
    }
}

void CopyOverlay::readSettings() {
    if (settings->panelEnabled() && settings->panelPosition() == PanelPosition::PANEL_BOTTOM) {
        setPosition(FloatingWidgetPosition::TOPLEFT);
    } else {
        setPosition(FloatingWidgetPosition::BOTTOMLEFT);
    }
    update();
}

void CopyOverlay::saveSettings() {
    // Collect paths from all widget pages and deduplicate
    QStringList newPaths;
    QStringList seen;
    
    for (int i = 0; i < pathWidgets.count(); i++) {
        QString dirPath = pathWidgets.at(i)->directory();
        if (!dirPath.isEmpty() && !seen.contains(dirPath)) {
            seen << dirPath;
            newPaths << dirPath;
        }
    }
    
    paths = newPaths;
    settings->setSavedPaths(paths);
}

void CopyOverlay::createDefaultPaths() {
    QString home = QDir::homePath();
    if (paths.count() < 1 || paths.at(0).isEmpty() || paths.at(0)[0] == '@') {
        paths.clear();
        paths << home;
    }
    if (paths.count() == 1 && paths.at(0) == home) {
        QDir dir(home);
        foreach(QFileInfo mfi, dir.entryInfoList()) {
            if (paths.count() >= itemsPerPage) break;
            if(mfi.isFile()) continue;
            if(mfi.fileName() == "." || mfi.fileName() == ".." || mfi.fileName()[0] == '.' || mfi.fileName() == "3D Objects" || mfi.fileName() == "Contacts" || mfi.fileName() == "Favorites" || mfi.fileName() == "Links" || mfi.fileName() == "Saved Games" || mfi.fileName() == "Searches") continue;
            QString qpath(home + "/" + mfi.fileName());
            QFileInfo qinfo(qpath);
            if (qinfo.permission(QFile::WriteUser | QFile::ReadGroup)) {
                paths << qpath;
            }
        }
    }
}

bool CopyOverlay::focusNextPrevChild(bool) {
    return false;
}

void CopyOverlay::keyPressEvent(QKeyEvent *event) {
    event->accept();
    QString key = actionManager->keyForNativeScancode(event->nativeScanCode());
    
    // Numeric shortcuts work on ALL pages now
    if (shortcuts.contains(key)) {
        int indexOnPage = shortcuts[key];
        int start = currentPage * itemsPerPage;
        
        // Check if this index points to a regular path widget
        int end = std::min(start + itemsPerPage, static_cast<int>(pathWidgets.count()));
        int itemsOnCurrentPage = end - start;
        
        if (indexOnPage < itemsOnCurrentPage) {
            // It's a regular path widget
            int globalIndex = start + indexOnPage;
            requestFileOperation(pathWidgets.at(globalIndex)->directory());
        } else if (indexOnPage == itemsOnCurrentPage) {
            // Check if there's an "Add New Path" item on this page
            // The "Add New Path" item appears on the last page if there's space
            if (!addPathWidgets.isEmpty() && currentPage == ui->stackedWidget->count() - 1) {
                // Trigger the add new path dialog
                addPathWidgets.first()->triggerDialog();
            }
        }
    } else {
        actionManager->processEvent(event);
    }
}
