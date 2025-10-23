#pragma once

#include "image.h"
#include <QMovie>
#include <QTimer>

class ImageAnimated : public Image {
public:
    ImageAnimated(QString _path);
    ImageAnimated(std::unique_ptr<DocumentInfo> _info);
    ~ImageAnimated();

    std::unique_ptr<QPixmap> getPixmap();
    std::shared_ptr<const QImage> getImage();
    std::shared_ptr<QMovie> getMovie();
    int height();
    int width();
    QSize size();

    bool isEditable();
    bool isEdited();

    int frameCount();
    void unload(); // Release file handle for move/rename/delete operations
    void load();   // Reload after unload to prepare for next operation
public slots:
    bool save();
    bool save(QString destPath);

signals:
    void frameChanged(QPixmap*);

private:
    QSize mSize;
    int mFrameCount;
    std::shared_ptr<QMovie> movie;
    void loadMovie();
};
