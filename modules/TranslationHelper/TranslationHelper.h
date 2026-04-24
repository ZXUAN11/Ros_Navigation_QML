/***********************************************************************************************************************
** TranslationHelper - 多语言翻译工具模块
** 独立子模块，可直接注册为 QML 单例使用，无需编写任何 wrapper 类。
**
** 使用方式（main.cpp 中）：
**   TranslationHelper translator;
**   qmlRegisterSingletonInstance("App", 1, 0, "Translator", &translator);
**   QQmlApplicationEngine engine;
**   translator.init(&engine, "MyApp");     // "MyApp" → 加载 MyApp_en_US.qm
**   engine.load(...);
**
** QML 中：
**   text: qsTr("中文文本")                  // 自动翻译
**   Translator.setLanguage(1)              // 切换到英文（0=中文, 1=英文）
**   console.log(Translator.languageName)   // "English"
***********************************************************************************************************************/

#ifndef TRANSLATIONHELPER_H
#define TRANSLATIONHELPER_H

#include <QObject>
#include <QPointer>
#include <QTranslator>

class QQmlApplicationEngine;

class TranslationHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int language READ language NOTIFY languageChanged)
    Q_PROPERTY(QString languageName READ languageName NOTIFY languageChanged)

public:
    enum Language
    {
        Chinese = 0,
        English = 1
    };
    Q_ENUM(Language)

    explicit TranslationHelper(QObject *parent = nullptr);
    ~TranslationHelper() override;

    /// 初始化：绑定 QML 引擎，根据系统语言自动加载翻译
    /// @param engine   QQmlApplicationEngine 实例
    /// @param appName  App 名称，用于拼接 .qm 文件名（如 "VTApp" → "VTApp_en_US.qm"）
    void init(QQmlApplicationEngine *engine, const QString &appName);

    /// 切换语言
    Q_INVOKABLE void setLanguage(int lang);

    int language() const;
    QString languageName() const;

    /// 检测系统语言
    static Language detectSystemLanguage();

Q_SIGNALS:
    void languageChanged();

private:
    void loadTranslation(Language lang);

    QTranslator mTranslator;
    QPointer<QQmlApplicationEngine> mEngine;
    Language mCurrentLanguage;
    QString mAppName;
    bool mInstalled;
};

#endif // TRANSLATIONHELPER_H
