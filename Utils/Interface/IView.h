#ifndef IWIDGET_H
#define IWIDGET_H

#include <QString>

class IView
{

protected:
    /**
     * @brief setupUI 初始化UI
     * @param parent
     */
    virtual void setupUI(class QWidget* parent) = 0;

    /**
     * @brief reTranslateUI 多语言切换
     */
    virtual void reTranslateUI() = 0;

public:

    /**
     * @brief initMenu 初始化菜单栏
     * @param menu
     */
    virtual void initMenu(class QMenu *menu) = 0;

    /**
     * @brief getViweName 获取view名称
     * @return
     */
    virtual QString getViweName() const = 0;

    /**
     * @brief getViewUrl 获取view的url
     * @return
     */
    virtual QString getViewUrl() const = 0;
};

#endif // IWIDGET_H
