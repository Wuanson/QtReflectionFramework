#ifndef REF_CLASSINFO_H
#define REF_CLASSINFO_H

/**
  * 在此文件封装Q_CLASSINFO
  */


#define KEY_MODEL_NAMES      "K_ModelNames"
#define KEY_VIEW_NAME       "K_ViewName"
#define KEY_VIEW_URL        "K_ViewUrl"
#define KEY_REF_INIT        "K_ReflectionInit"
#define KEY_MAIN_WINDOW     "K_MainWindow"
#define KEY_VIEW            "K_View"
#define KEY_MODEL           "K_Model"
#define KEY_SYSTEM          "K_System"
#define KEY_REFFLECTION_INIT "K_ReflectionInit"


/**
  *@brief Q_REF_INIT 标记初始化类
  */
#define Q_REF_INIT                  Q_CLASSINFO(KEY_REFFLECTION_INIT,"true")

/**
  *@brief Q_VIEW 视图类
  */
#define Q_VIEW                      Q_CLASSINFO(KEY_VIEW,"true")

/**
  *@brief Q_MODEL 标记模型类
  */
#define Q_MODEL                     Q_CLASSINFO(KEY_MODEL,"true")

/**
  *@brief Q_SYSTEM 标记系统类
  */
#define Q_SYSTEM                    Q_CLASSINFO(KEY_SYSTEM,"true")

/**
  *@brief Q_MODEL_NAMES 标记 Model 的 Property 名称 多个名称之间使用 ; 号隔开
  */
#define Q_MODEL_NAMES(NAME)          Q_CLASSINFO(KEY_MODEL_NAMES,#NAME)

/**
  *@brief Q_VIEW_NAME 标记QML的名称
  */
#define Q_VIEW_NAME(NAME)           Q_CLASSINFO(KEY_VIEW_NAME,NAME)

/**
  *@brief Q_VIEW_URL 标记QML的URL
  */
#define Q_VIEW_URL(NAME)            Q_CLASSINFO(KEY_VIEW_URL,NAME)

/**
  *@brief Q_MAIN_WINDOW 标记QWidget中的MainWindow类
  */
#define Q_MAIN_WINDOW               Q_VIEW \
                                    Q_CLASSINFO(KEY_MAIN_WINDOW,"true")


#endif // REF_CLASSINFO_H
