//
// Created by sam on 3/29/17.
//

#ifndef MYAPPLICATION_IND_H
#define MYAPPLICATION_IND_H

#include <string>

//using namespace std;
//#include <android/log.h>
//#define TAG    "ind-jni" // 这个是自定义的LOG的标识
#define LOGD(...) // __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__) // 定义LOGD类型
//
#define LogTrace(...)  //__android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__) // 定义LOGD类型

#define __NAN NAN

const char* my_test();
typedef unsigned int DWORD;
#define ZeroMemory(a, b) memset(a, 0, b)

#define MAX_PARAM_COUNT 7

enum LineColor {
    LineColorIncreasing = 1,
    LineColorNetural    = 0,
    LineColorDecreasing = -1
};

struct CFDayMobile
{
    CFDayMobile()
    {
        ZeroMemory(this, sizeof(CFDayMobile));
    }

    void Clear()
    {
        ZeroMemory(this, sizeof(CFDayMobile));
    }

    DWORD m_dwTime;

    double m_fOpen;
    double m_fHigh;
    double m_fLow;
    double m_fClose;
    double m_fVolume;
    double m_fAmount;
    double m_fLTG;

    double m_pfInd[MAX_PARAM_COUNT];
    double m_color;

};


class CInd //: public  Loki::SmallObject<Loki::ClassLevelLockable>
{
public:
    CInd();
    virtual ~CInd(){};

public:
    virtual void Calc(CFDayMobile* pFDay, int nNum) = 0;
    virtual std::string GetOutlineName(int index);   // 输出线名
    virtual std::string GetParamName(int index);     // 输入参数名
    virtual int GetColoredIndIndex();
    virtual int GetOutlineCount();
    virtual int GetBeginIndex(int outlineIndex);
    
    virtual void SetParamSize(char size);
    virtual void SetParam(short p, int index);
    virtual short GetParam(int index);

protected:
    char m_cParamSize;
    short m_psParam[MAX_PARAM_COUNT];

    char m_cExpSize;     //返回的指标根数，最多六根，一般在calc里面设置
    int m_pnFirst[MAX_PARAM_COUNT];
    int m_coloredIndIndex; // 默认-1 表示没有需要着色的线 如果>=0 通过index为对应的ind着色
    std::string _indParamNames[MAX_PARAM_COUNT]; // TODO
    std::string _indOutlineNames[MAX_PARAM_COUNT];
};


class CInd_MA : public CInd
{
public:
    CInd_MA();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};


class CInd_VMA : public CInd
{
public:
    CInd_VMA();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

class CInd_MACD : public CInd
{
public:
    CInd_MACD();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

class CInd_KDJ : public CInd
{
public:
    CInd_KDJ();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

class CInd_RSI : public CInd
{
public:
    CInd_RSI();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

class CInd_WR : public CInd
{
public:
    CInd_WR();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

class CInd_VR : public CInd
{
public:
    CInd_VR();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//资金流变
class CIndL2_ZJLB : public CInd
{
public:
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};


//趋向指标
class CInd_DMI : public CInd
{
public:
    CInd_DMI();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//平均差
class CInd_DMA : public CInd
{
public:
    CInd_DMA();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//三重指数平均线
class CInd_TRIX : public CInd
{
public:
    CInd_TRIX();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//情绪指标
class CInd_BRAR : public CInd
{
public:
    CInd_BRAR();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//带状能量线
class CInd_CR : public CInd
{
public:
    CInd_CR();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//累计能量线
class CInd_OBV : public CInd
{
public:
    CInd_OBV();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//振动升降指标
class CInd_ASI : public CInd
{
public:
    CInd_ASI();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//简易波动指标
class CInd_EMV : public CInd
{
public:
    CInd_EMV();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//商品路径指标
class CInd_CCI : public CInd
{
public:
    CInd_CCI();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//变动率指标
class CInd_ROC : public CInd
{
public:
    CInd_ROC();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//动量线
class CInd_MTM : public CInd
{
public:
    CInd_MTM();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//心理线
class CInd_PSY : public CInd
{
public:
    CInd_PSY();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//布林线
class CInd_BOLL : public CInd
{
public:
    CInd_BOLL();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};



//抛物线指标
class CInd_SAR : public CInd
{
public:
    CInd_SAR();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};



class CDayCostPV
{
public:
    CDayCostPV()
    {
        Initial();
    }

    void Initial()
    {
        m_fPrice = 0;
        m_fRedVolume = 0;
        m_fGreenVolume = 0;
    };

    double m_fPrice;
    double m_fRedVolume;
    double m_fGreenVolume;
};





class CInd_QSDD : public CInd
{
public:
    CInd_QSDD();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};




// 增加EXPMA, SLOWKD, ENE, BIAS指标 [8/19/2016 qianyifan]
class CInd_EMA : public CInd
{
public:
    CInd_EMA();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

class CInd_SLOWKD : public CInd
{
public:
    CInd_SLOWKD();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

class CInd_ENE : public CInd
{
public:
    CInd_ENE();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};


class CInd_BIAS : public CInd
{
public:
    CInd_BIAS();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

// 不接受输入参数
class CInd_FSL : public CInd
{
public:
    CInd_FSL();
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};


#endif //MYAPPLICATION_IND_H
