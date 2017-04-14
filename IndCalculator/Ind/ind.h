//
// Created by sam on 3/29/17.
//

#ifndef MYAPPLICATION_IND_H
#define MYAPPLICATION_IND_H

#include <string.h>

#define LOGD(...)   // 定义LOGD类型
#define LogTrace(...)

//const char* my_test();
typedef unsigned int DWORD;
#define ZeroMemory(a, b) memset(a, 0, b)

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

//    DWORD	m_dwNumOfBuy;
//    DWORD	m_dwNumOfSell;
//
//    double	m_fAmtOfBuy[3];
//    double	m_fAmtOfSell[3];

//    double	m_fVolOfBuy[3];
//    double	m_fVolOfSell[3];

    double m_pfMA[6];
    double m_pfVMA[6];

    double m_pfInd[6];

    char m_cCPX;
};


class CInd //: public  Loki::SmallObject<Loki::ClassLevelLockable>
{
public:
    CInd();
    virtual ~CInd(){};

public:
    virtual void Calc(CFDayMobile* pFDay, int nNum) = 0;

protected:
    virtual char GetGroup();


public:
    char m_cParamSize;
    short m_psParam[6];

    char m_cExpSize; //返回的指标根数，最多六根，一般在calc里面设置
    int m_pnFirst[6];
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
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

class CInd_KDJ : public CInd
{
public:
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

class CInd_RSI : public CInd
{
public:
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

class CInd_WR : public CInd
{
public:
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

class CInd_VR : public CInd
{
public:
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
    virtual void Calc(CFDayMobile* pFDay, int nNum);
};

//振动升降指标
class CInd_ASI : public CInd
{
public:
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



#endif //MYAPPLICATION_IND_H

