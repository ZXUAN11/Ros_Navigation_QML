/***********************************************************************************************************************
** TranslationHelper - 多语言翻译工具模块
***********************************************************************************************************************/

#include "TranslationHelper.h"

#include <QCoreApplication>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QDebug>

TranslationHelper::TranslationHelper(QObject *parent)
    : QObject(parent)
    , mCurrentLanguage(Chinese)
    , mInstalled(false)
{
    mCurrentLanguage = detectSystemLanguage();
}

TranslationHelper::~TranslationHelper()
{
    if (mInstalled)
    {
        qApp->removeTranslator(&mTranslator);
    }
}

void TranslationHelper::init(QQmlApplicationEngine *engine, const QString &appName)
{
    mEngine = engine;
    mAppName = appName;
    loadTranslation(mCurrentLanguage);
}

void TranslationHelper::setLanguage(int lang)
{
    auto target = static_cast<Language>(lang);
    if (mCurrentLanguage == target)
    {
        return;
    }
    loadTranslation(target);
    emit languageChanged();
}

int TranslationHelper::language() const
{
    return static_cast<int>(mCurrentLanguage);
}

QString TranslationHelper::languageName() const
{
    switch (mCurrentLanguage)
    {
    case Chinese: return QStringLiteral("中文");
    case English: return QStringLiteral("English");
    default:      return QString();
    }
}

TranslationHelper::Language TranslationHelper::detectSystemLanguage()
{
    const QLocale systemLocale = QLocale::system();
    return systemLocale.language() == QLocale::Chinese ? Chinese : English;
}

void TranslationHelper::loadTranslation(Language lang)
{
    // 1. 卸载旧翻译
    if (mInstalled)
    {
        qApp->removeTranslator(&mTranslator);
        mInstalled = false;
    }

    // 2. 中文是 source text 默认语言，无需加载翻译文件
    if (lang != Chinese)
    {
        const QString qmFile = QString("%1_en_US").arg(mAppName);

        // 优先从 qrc 资源中加载
        if (mTranslator.load(qmFile, QStringLiteral(":/i18n")))
        {
            qApp->installTranslator(&mTranslator);
            mInstalled = true;
        }
        else
        {
            //从可执行文件同级 translations/ 目录加载
            const QString appDir = QCoreApplication::applicationDirPath();
            if (mTranslator.load(qmFile, appDir + QStringLiteral("/translations")))
            {
                qApp->installTranslator(&mTranslator);
                mInstalled = true;
            }
            else
            {
                qWarning() << "TranslationHelper: Failed to load translation:" << qmFile;
            }
        }
    }

    mCurrentLanguage = lang;

    // 3. 通知 QML 引擎刷新所有 qsTr()
    if (mEngine)
    {
        mEngine->retranslate();
    }
}
