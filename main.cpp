#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "wordsstatisticsmodel.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QScopedPointer<WordsStatisticsModel> wordsModel(new WordsStatisticsModel);
    QScopedPointer<Controller> controller(new Controller);

    wordsModel->connect(controller.get(), &Controller::sigProcessWord, wordsModel.get(), &WordsStatisticsModel::appendWord);
    wordsModel->connect(controller.get(), &Controller::sigStarted, wordsModel.get(), &WordsStatisticsModel::clearModel);

    qmlRegisterSingletonInstance("WordModelInstance", 1, 0, "WordModelInstance", wordsModel.get());
    qmlRegisterSingletonInstance("ControllerInstance", 1, 0, "ControllerInstance", controller.get());

    const QUrl url(u"qrc:/words_statistics/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);
    return app.exec();
}

/*
Тестовое:
Напишите приложение на Qt с использованием QML, которое считывает слова из файла и выводит топ 15 слов (по количеству вхождений) в режиме реального времени в виде гистограммы.
Приложение должно быть многопоточным - чтение и обработка файла должны происходить в разных потоках, гистограмма должна обновляться в условно реальном времени
(без видимых продолжительных задержек). Интерфейс приложения должен быть адаптивным и учитывать размеры текущего окна.
Приложение должно содержать следующие элементы интерфейса:
•	гистограмма (нежелательно использовать ChartView из модуля charts)
•	прогресс бар обработки открытого файла
•	кнопка "Открыть" для выбора файла, из которого будут считаны слова.
•	кнопка "Старт" для запуска чтения файла и подсчета статистики слов
•	кнопка "Пауза" для приостановки/возобновления чтения файла (опционально: наличие кнопки будет плюсом)
•	кнопка "Отмена" для прерывания чтения файла и сброса статистики (опционально: наличие кнопки будет плюсом)
Плюсом будет наличие readme файла с описанием архитектуры и логики работы приложения.
 */
