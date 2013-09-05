#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>
#include <QDebug>
#include <QElapsedTimer>

#include "rapidjson/document.h"		// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h"	// for stringify JSON
#include "rapidjson/filestream.h"	// wrapper of C stream for prettywriter as output

struct DeskWallpaper {
    int width;
    int height;
    QString url;

    int previewWidth;
    int previewHeight;
    QString previewUrl;

    int thumbWidth;
    int thumbHeight;
    QString thumbUrl;

    void printAll() {
        qDebug() << "Image width: " << width;
        qDebug() << "Image height: " << height;
        qDebug() << "Image url: " << url;

        qDebug() << "Preview width: " << previewWidth;
        qDebug() << "Preview height: " << previewHeight;
        qDebug() << "Preview url: " << previewUrl;

        qDebug() << "Thumb width: " << thumbWidth;
        qDebug() << "Thumb height: " << thumbHeight;
        qDebug() << "Thumb url: " << thumbUrl;
    }
};

struct Availability {
    QString date;
    QString type;
    bool delimiter;
    QString del;
};

int testHuge() {
    QFile jsonFile("huge.json");

    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file, exiting...";
        return 1005;
    }

    QByteArray jsonData = jsonFile.readAll();

    QElapsedTimer timer;
    timer.start();

    for (int i = 0; i < 10000; i++)
    {
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);

        QVariantMap mainMap = doc.object().toVariantMap();

        Availability a;

        foreach (QVariant object, mainMap["availability"].toList())
        {
            QVariantMap av = object.toMap();
            a.date = av["date"].toString();
            a.type = av["type"].toString();
            a.delimiter = av["delimiter"].toBool();
        }
    }

    qDebug() << timer.nsecsElapsed();
}

int demo() {
    QFile jsonFile("random.json");

    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file, exiting...";
        return 1005;
    }

    QByteArray jsonData = jsonFile.readAll();

    QJsonParseError *err = new QJsonParseError();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData, err);

    if (err->error != 0)
        qDebug() << err->errorString();

    DeskWallpaper wallpaper;

    if (doc.isNull())
    {
        qDebug() << "Invalid json document, exiting...";
        return 1006;
    }
    else if (doc.isObject())
    {
        //get the jsonObject
        QJsonObject jObject = doc.object();

        //convert the json object to variantmap
        QVariantMap mainMap = jObject.toVariantMap();

        //get the "response" variant map
        QVariantMap responseMap = mainMap["response"].toMap();

        //get the width and height of image
        wallpaper.width = responseMap["width"].toInt();
        wallpaper.height = responseMap["height"].toInt();

        //get the image url
        QVariantMap imageMap = responseMap["image"].toMap();
        wallpaper.url = imageMap["url"].toString();

        //get the preview image details
        QVariantMap previewMap = imageMap["preview"].toMap();
        wallpaper.previewWidth = previewMap["width"].toInt();
        wallpaper.previewHeight = previewMap["height"].toInt();
        wallpaper.previewUrl = previewMap["url"].toString();

        //get the thumbnail details
        QVariantMap thumbMap = imageMap["thumb"].toMap();
        wallpaper.thumbWidth = thumbMap["width"].toInt();
        wallpaper.thumbHeight = thumbMap["height"].toInt();
        wallpaper.thumbUrl = thumbMap["url"].toString();

        wallpaper.printAll();
    }
    else if (doc.isArray())
    {
        qDebug() << "Are you sure this is the json from Desktoppr? exiting...";
        return 1007;
    }
    else if (doc.isEmpty())
    {
        qDebug() << "Empty json document, exiting...";
        return 1008;
    }
    else
    {
        qDebug() << "Unable to determine type of JSON, exiting...";
        return 1009;
    }
}

int testRapid()
{
    rapidjson::Document document;

    QFile jsonFile("huge.json");

    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file, exiting...";
        return 1005;
    }

    QByteArray jsonData = jsonFile.readAll();

    QElapsedTimer timer;
    timer.start();

    for (int i = 0; i < 10000; i++)
    {
        Availability av;

        if (document.Parse<0>(jsonData).HasParseError())
            return 1;

//        qDebug() << document.IsObject() << 1;

        const rapidjson::Value& a = document["availability"];

//        qDebug() << a.IsArray() << 2;

        for (rapidjson::SizeType j = 0; j < a.Size(); j++)
        {
            const rapidjson::Value& o = a[j];
             av.date = QString::fromStdString(o["date"].GetString());
             av.type = QString::fromStdString(o["type"].GetString());
             av.del = QString::fromStdString(o["delimiter"].GetString());
        }
    }

    qDebug() << timer.nsecsElapsed();
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

//    testRapid();
    testHuge();

    exit(1);
    return app.exec();
}
