//
// Created by sam on 3/29/17.
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <new>
#include "ind.h"

#define Max(a,b) (((a)>(b))?(a):(b))
#define Min(a,b) (((a)<(b))?(a):(b))

typedef bool BOOL;

// consts
#define TRUE	true
#define FALSE	false

#define EPS (1e-5)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __SUM(double *input, int count, int period, double *output) {
    for (int i = 0; i < count; i++) {
        int sum = __NAN;
        for (int j = Max(i - period, 0); j <= i; j++) {
            if (j < 0) {
                continue;
            }
            if (!isnan(input[j])) {
                sum = 0;
            }
            
            sum += input[j];
        }
        
        output[i] = sum;
    }
}

void Cal_VMAColor(CFDayMobile* pDay, int nDay) {
    if (pDay == NULL || nDay <= 0) return;
    for (int i = 0; i < nDay; i++) {
        CFDayMobile candle = pDay[i];
        if (candle.m_fClose > candle.m_fOpen) {
            pDay[i].m_color = LineColorIncreasing;
        } else if (candle.m_fClose < candle.m_fOpen) {
            pDay[i].m_color = LineColorDecreasing;
        } else {
            if (i >= 1) {
                CFDayMobile preCandle = pDay[i-1];
                if (candle.m_fClose >= preCandle.m_fClose) {
                    pDay[i].m_color = LineColorIncreasing;
                } else {
                    pDay[i].m_color = LineColorDecreasing;
                }
                
            } else {
                pDay[i].m_color = LineColorIncreasing;
            }
        }
    }
}

void Calc_MA(double* pfMA, CFDayMobile* pDay, int nDay, short sParam)
{
    if (pDay == NULL || nDay <= 0) return;

    for (int i = 0; i<nDay; i++)
    {
        if (i < sParam - 1)
            pfMA[i] = __NAN;
        else
        {
            pfMA[i] = 0;
            for (int j=i-sParam+1; j<=i; j++) {
                pfMA[i] += pDay[j].m_fClose;
            }
            pfMA[i] /= sParam;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Calc_EMA(double* pfEMA, double* pfValue, int nValue, short sParam)
{
    LogTrace("%s", __FUNCTION__);
    if (pfEMA && pfValue && nValue>0 && sParam>0)
    {
        pfEMA[0] = pfValue[0];
        for (int i = 1; i < nValue; i++)
            pfEMA[i] = (pfEMA[i-1] * (sParam-1) + pfValue[i] * 2) / (sParam + 1);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Calc_LLV(double* pfLLV, CFDayMobile* pDay, int nDay, short sParam)
{
    LogTrace("%s", __FUNCTION__);
    if (pfLLV && pDay && nDay>0 && sParam>0)
    {
        int nFirst = int(sParam-1);

        for (int i=0; i<nDay; i++)
        {
            if (i == 0)
                pfLLV[i] = pDay[i].m_fLow;
            else if (i < nFirst)
            {
                if (pfLLV[i-1] > pDay[i].m_fLow)
                    pfLLV[i] = pDay[i].m_fLow;
                else
                    pfLLV[i] = pfLLV[i-1];
            }
            else
            {
                int nFront = i-sParam+1;
                pfLLV[i] = pDay[nFront].m_fLow;

                for (int j=nFront+1; j<=i; j++)
                {
                    if (pfLLV[i]>pDay[j].m_fLow)
                        pfLLV[i] = pDay[j].m_fLow;
                }
            }
        }
    }
}

void Calc_LLV(int index, CFDayMobile* pDay, int nDay, short sParam)
{
    LogTrace("%s", __FUNCTION__);
    if (pDay && nDay>0 && sParam>0)
    {
        int nFirst = int(sParam-1);

        for (int i=0; i<nDay; i++)
        {
            if (i == 0)
                pDay[i].m_pfInd[index] = pDay[i].m_fLow;
            else if (i < nFirst)
            {
                if (pDay[i-1].m_pfInd[index] > pDay[i].m_fLow)
                    pDay[i].m_pfInd[index] = pDay[i].m_fLow;
                else
                    pDay[i].m_pfInd[index] = pDay[i-1].m_pfInd[index];
            }
            else
            {
                int nFront = i-sParam+1;
                pDay[i].m_pfInd[index] = pDay[nFront].m_fLow;

                for (int j=nFront+1; j<=i; j++)
                {
                    if (pDay[i].m_pfInd[index] > pDay[j].m_fLow)
                        pDay[i].m_pfInd[index] = pDay[j].m_fLow;
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Calc_HHV(double* pfHHV, CFDayMobile* pDay, int nDay, short sParam)
{
    LogTrace("%s", __FUNCTION__);
    if (pfHHV && pDay && nDay>0 && sParam>0)
    {
        int nFirst = int(sParam-1);

        for (int i=0; i<nDay; i++)
        {
            if (i == 0)
                pfHHV[i] = pDay[i].m_fHigh;
            else if(i < nFirst)
            {
                if (pfHHV[i-1] < pDay[i].m_fHigh)
                    pfHHV[i] = pDay[i].m_fHigh;
                else
                    pfHHV[i] = pfHHV[i-1];
            }
            else
            {
                int nFront = i-sParam+1;
                pfHHV[i] = pDay[nFront].m_fHigh;

                for (int j=nFront+1; j<=i; j++)
                {
                    if (pfHHV[i]<pDay[j].m_fHigh)
                        pfHHV[i] = pDay[j].m_fHigh;
                }
            }
        }
    }
}

void Calc_HHV(int index, CFDayMobile* pDay, int nDay, short sParam)
{
    LogTrace("%s", __FUNCTION__);
    if (pDay && nDay>0 && sParam>0)
    {
        int nFirst = int(sParam-1);

        for (int i=0; i<nDay; i++)
        {
            if (i == 0)
                pDay[i].m_pfInd[index] = pDay[i].m_fHigh;
            else if (i < nFirst)
            {
                if (pDay[i-1].m_pfInd[index] < pDay[i].m_fHigh)
                    pDay[i].m_pfInd[index] = pDay[i].m_fHigh;
                else
                    pDay[i].m_pfInd[index] = pDay[i-1].m_pfInd[index];
            }
            else
            {
                int nFront = i-sParam+1;
                pDay[i].m_pfInd[index] = pDay[nFront].m_fHigh;

                for (int j=nFront+1; j<=i; j++)
                {
                    if (pDay[i].m_pfInd[index] < pDay[j].m_fHigh)
                        pDay[i].m_pfInd[index] = pDay[j].m_fHigh;
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void Calc_SMA(double* pfSMA, double* pfValue, int nValue, short sParam1, short sParam2, int nStart)
{
    LogTrace("%s", __FUNCTION__);
    if (pfSMA && pfValue && nValue>0 && sParam1>0 && sParam2>0 && nStart>=0)
    {
        if (nStart >=0 && nStart < nValue)
            pfSMA[nStart] = pfValue[nStart];
        for (int i=nStart+1; i<nValue; i++)
            pfSMA[i] = (pfSMA[i-1]*(sParam1-sParam2)+pfValue[i]*sParam2)/sParam1;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Calc_SUM(double* pfSum, double* pfValue, int nValue, short sParam, int nStart, int& nFirst)
{
    LogTrace("%s", __FUNCTION__);
    if (pfSum && pfValue && nValue>0 && sParam>0 && nStart>=0)
    {
        nFirst = int(nStart+sParam-1);

        for (int i=nStart; i<nValue; i++)
        {
            pfSum[i] = 0;
            if (i == nStart)
                pfSum[i] = pfValue[i];
            else if(i < nFirst)
                for (int j=nStart; j<=i; j++)
                    pfSum[i] += pfValue[j];
            else
                for (int j=i-sParam+1; j<=i; j++)
                    pfSum[i] += pfValue[j];
        }
    }
}


//////////////////////////////////////////////////////////////////////

CInd::CInd()
{
    m_cParamSize = 0;
    ZeroMemory(m_psParam, MAX_PARAM_COUNT * sizeof(short));

    m_cExpSize = 0;
    ZeroMemory(m_pnFirst, MAX_PARAM_COUNT * sizeof(int));
    
    m_coloredIndIndex = -1;  
}

void CInd::Calc(CFDayMobile* pFDay, int nNum)
{

}

std::string CInd::GetOutlineName(int index) {
    return _indOutlineNames[index];
}

std::string CInd::GetParamName(int index) {
    return _indParamNames[index];
}

int CInd::GetColoredIndIndex() {
    return m_coloredIndIndex;
}

int CInd::GetOutlineCount() {
    return m_cExpSize;
}

int CInd::GetBeginIndex(int outlineIndex) {
    return m_pnFirst[outlineIndex];
}

void CInd::SetParamSize(char size) {
    m_cParamSize = size;
}

void CInd::SetParam(short p, int index) {
    m_psParam[index] = p;
}

short CInd::GetParam(int index) {
    return m_psParam[index];
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInd_MA::CInd_MA() : CInd()
{
    _indParamNames[0] = "P1";
    _indParamNames[1] = "P2";
    _indParamNames[2] = "P3";
    _indParamNames[3] = "P4";
    _indParamNames[4] = "P5";
    _indParamNames[5] = "P6";

    _indOutlineNames[0] = "MA1";
    _indOutlineNames[1] = "MA2";
    _indOutlineNames[2] = "MA3";
    _indOutlineNames[3] = "MA4";
    _indOutlineNames[4] = "MA5";
    _indOutlineNames[5] = "MA6";
    
    m_psParam[0] = 5;
    m_psParam[1] = 10;
    m_psParam[2] = 20;
    m_psParam[3] = 30;
    m_psParam[4] = 60;
    m_psParam[5] = 120;
}

void CInd_MA::Calc(CFDayMobile* pFDay, int nNum)
{
    LOGD("%s", __FUNCTION__);

    m_cExpSize = m_cParamSize;

    if (pFDay == NULL || nNum <= 0) return;

    for (int c=0; c<m_cParamSize; c++)
    {
        short sParam = m_psParam[c];
        if (sParam<1)
            sParam = 1;

        m_pnFirst[c] = int(sParam-1);  //设置前置K线根数

        for (int i=0; i<nNum; i++)
        {
            if (i < m_pnFirst[c])
                pFDay[i].m_pfInd[c] = __NAN;
            else if (sParam < 2)
                pFDay[i].m_pfInd[c] = __NAN;
            else
            {
                pFDay[i].m_pfInd[c] = 0;
                for (int j= i- sParam + 1; j<=i; j++)
                    pFDay[i].m_pfInd[c] += pFDay[j].m_fClose;
                pFDay[i].m_pfInd[c] /= sParam;
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInd_VMA::CInd_VMA() : CInd()
{
    _indParamNames[0] = "M1";
    _indParamNames[1] = "M2";
    _indParamNames[2] = "M3";
    _indParamNames[3] = "M4";
    _indParamNames[4] = "M5";
    _indParamNames[5] = "M6";

    _indOutlineNames[0] = "VOLUME";
    _indOutlineNames[1] = "MA1";
    _indOutlineNames[2] = "MA2";
    _indOutlineNames[3] = "MA3";
    _indOutlineNames[4] = "MA4";
    _indOutlineNames[5] = "MA5";
    _indOutlineNames[6] = "MA6";
    
    m_psParam[0] = 5;
    m_psParam[1] = 10;
    m_psParam[2] = 20;
    m_psParam[3] = 30;
    m_psParam[4] = 60;
    m_psParam[5] = 120;
}

void CInd_VMA::Calc(CFDayMobile* pFDay, int nNum)
{
    LogTrace("%s", __FUNCTION__);

    //ASSERT(m_cParamSize<=5);
    m_cExpSize = m_cParamSize + 1;
    m_coloredIndIndex = 0;
    
    if (pFDay == NULL || nNum <= 0) return;

    for (int c=0; c<m_cParamSize+1; c++)
    {
        if (c == 0)
        {
            m_pnFirst[c] = 0;
            for (int i = 0; i<nNum; i++)
            {
                pFDay[i].m_pfInd[c] = pFDay[i].m_fVolume;
            }
            
            Cal_VMAColor(pFDay, nNum);
        }
        else
        {
            short sParam = m_psParam[c-1];
            if (sParam < 1)
                sParam = 1;

            m_pnFirst[c] = int(sParam-1);

            for (int i = 0; i<nNum; i++)
            {
                if (i < m_pnFirst[c])
                    pFDay[i].m_pfInd[c] = __NAN;
                else if (sParam < m_cParamSize)
                    pFDay[i].m_pfInd[c] = __NAN;
                else
                {
                    pFDay[i].m_pfInd[c] = pFDay[i].m_pfInd[c] = 0;
                    for (int j=i-sParam+1; j<=i; j++)
                    {
                        pFDay[i].m_pfInd[c] += pFDay[j].m_fVolume;
                    }
                    pFDay[i].m_pfInd[c] /= sParam;
                }
            }
        }
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInd_MACD::CInd_MACD() {
    // DIF", "DEA", "MACD"
    _indParamNames[0] = "LONG";
    _indParamNames[1] = "SHORT";
    _indParamNames[2] = "M";
    
    _indOutlineNames[0] = "DIFF";
    _indOutlineNames[1] = "DEA";
    _indOutlineNames[2] = "MACD";
}

void CInd_MACD::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize==3);
    m_cExpSize = 3;

    if (pFDay == NULL || nNum <= 0) return;

    double* pfClose = new(std::nothrow) double[nNum];	ZeroMemory(pfClose, nNum*sizeof(double));
    double* pfEMA_Long = new(std::nothrow) double[nNum];ZeroMemory(pfEMA_Long, nNum*sizeof(double));
    double* pfEMA_Short = new(std::nothrow) double[nNum];ZeroMemory(pfEMA_Short, nNum*sizeof(double));

    if (pfClose && pfEMA_Long && pfEMA_Short)
    {
        for (int i=0; i<nNum; i++)
            pfClose[i] = pFDay[i].m_fClose;

        Calc_EMA(pfEMA_Long, pfClose, nNum, m_psParam[0]);
        Calc_EMA(pfEMA_Short, pfClose, nNum, m_psParam[1]);

        double* pfDif = pfClose;	// pfClose没用了
        for (int i=0; i<nNum; i++)
            pfClose[i] = pfEMA_Short[i]-pfEMA_Long[i];

        double* pfDea = pfEMA_Long;	// pfEMA_Long没用了
        Calc_EMA(pfDea, pfDif, nNum, m_psParam[2]);

        for (int i=0; i<nNum; i++)
        {
            pFDay[i].m_pfInd[0] = pfDif[i];
            pFDay[i].m_pfInd[1] = pfDea[i];
            pFDay[i].m_pfInd[2] = 2*(pfDif[i] - pfDea[i]);
        }
    }

    if (pfClose)
        delete [] pfClose;
    if (pfEMA_Long)
        delete [] pfEMA_Long;
    if (pfEMA_Short)
        delete [] pfEMA_Short;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RSV = (CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100;
//K : SMA(RSV,M1,1);
//D : SMA(K,M2,1);
//J : 3*K-2*D;
CInd_KDJ::CInd_KDJ() {
    
    _indParamNames[0] = "N";
    _indParamNames[1] = "M1";
    _indParamNames[2] = "M2";

    _indOutlineNames[0] = "K";
    _indOutlineNames[1] = "D";
    _indOutlineNames[2] = "J";
}

void CInd_KDJ::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize==3);
    m_cExpSize = 3;

    if (pFDay == NULL || nNum <= 0) return;

    double* pfLLV = new(std::nothrow) double[nNum];ZeroMemory(pfLLV, nNum*sizeof(double));
    double* pfHHV = new(std::nothrow) double[nNum];ZeroMemory(pfHHV, nNum*sizeof(double));
    double* pfRSV = new(std::nothrow) double[nNum];ZeroMemory(pfRSV, nNum*sizeof(double));

    if (pfLLV && pfHHV && pfRSV)
    {
        int nN = m_psParam[0];
        int nM1 = m_psParam[1];
        int nM2 = m_psParam[2];

        Calc_LLV(pfLLV, pFDay, nNum, nN);
        Calc_HHV(pfHHV, pFDay, nNum, nN);

        int nFirst;
        if (nN-1>0)
            nFirst = nN-1;
        else
            nFirst = 0;
        for (int i=0; i<nNum; i++)
        {
            if (pfHHV[i]!=pfLLV[i])
                pfRSV[i] = (pFDay[i].m_fClose-pfLLV[i])/(pfHHV[i]-pfLLV[i])*100;
            else if (i==0)
                pfRSV[i] = 0;
            else
                pfRSV[i] = pfRSV[i-1];
        }

        m_pnFirst[0] = 0;
        double* pfK = pfLLV;
        Calc_SMA(pfK, pfRSV, nNum, nM1, 1, 0);


        m_pnFirst[1] = 0;
        double* pfD = pfHHV;
        Calc_SMA(pfD, pfK, nNum, nM2, 1, 0);

        for (int i=0; i<nNum; i++)
        {
            pFDay[i].m_pfInd[0] = pfK[i];
            pFDay[i].m_pfInd[1] = pfD[i];
            pFDay[i].m_pfInd[2] = 3*pfK[i] - 2*pfD[i];
        }
    }

    if (pfLLV)
        delete [] pfLLV;
    if (pfHHV)
        delete [] pfHHV;
    if (pfRSV)
        delete [] pfRSV;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInd_RSI::CInd_RSI() {
    _indParamNames[0] = "N1";
    _indParamNames[1] = "N2";
    _indParamNames[2] = "N3";
    
    _indOutlineNames[0] = "RSI1";
    _indOutlineNames[1] = "RSI2";
    _indOutlineNames[2] = "RSI3";
}

void CInd_RSI::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize==2);
    m_cExpSize = m_cParamSize;

    if (pFDay == NULL || nNum <= 0) return;

    double* pfMax = new(std::nothrow) double[nNum];ZeroMemory(pfMax, nNum*sizeof(double));
    double* pfAbs = new(std::nothrow) double[nNum];ZeroMemory(pfAbs, nNum*sizeof(double));
    double* pfMaxSMA = new(std::nothrow) double[nNum];ZeroMemory(pfMaxSMA, nNum*sizeof(double));
    double* pfAbsSMA = new(std::nothrow) double[nNum];ZeroMemory(pfAbsSMA, nNum*sizeof(double));

    if (pfMax && pfAbs && pfMaxSMA && pfAbsSMA)
    {
        for (int c = 0; c < m_cParamSize; c++)
        {
            int nN = m_psParam[c];

            for (int i=1; i<nNum; i++)
            {
                if (pFDay[i].m_fClose >= pFDay[i-1].m_fClose)
                {
                    pfMax[i] = pFDay[i].m_fClose - pFDay[i-1].m_fClose;
                    pfAbs[i] = pFDay[i].m_fClose - pFDay[i-1].m_fClose;
                }
                else
                {
                    pfMax[i] = 0;
                    pfAbs[i] = pFDay[i-1].m_fClose - pFDay[i].m_fClose;
                }
            }

            Calc_SMA(pfMaxSMA, pfMax, nNum, nN, 1, 1);
            Calc_SMA(pfAbsSMA, pfAbs, nNum, nN, 1, 1);

            m_pnFirst[c] = 1;
            for (int i=1; i<nNum; i++)
            {
                if (pfAbsSMA[i] != 0)
                    pFDay[i].m_pfInd[c] = pfMaxSMA[i]/pfAbsSMA[i] * 100;
                else if (i == 1)
                    pFDay[i].m_pfInd[c] = __NAN;
                else
                    pFDay[i].m_pfInd[c] = pFDay[i-1].m_pfInd[c];
            }
        }
    }

    if (pfMax)
        delete [] pfMax;
    if (pfAbs)
        delete [] pfAbs;
    if (pfMaxSMA)
        delete [] pfMaxSMA;
    if (pfAbsSMA)
        delete [] pfAbsSMA;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInd_WR::CInd_WR() {
    _indParamNames[0] = "N1";
    _indParamNames[1] = "N2";
    _indParamNames[2] = "N3";

    _indOutlineNames[0] = "WR";
}

void CInd_WR::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize==1);
    m_cExpSize = m_cParamSize;

    if (pFDay == NULL || nNum <= 0) return;

    for(int c=0; c<m_cParamSize; c++) {
        int nN = m_psParam[c];
        if (nN < 1)
            nN = 1;

        Calc_LLV(c+1, pFDay, nNum, nN);
        Calc_HHV(c+2, pFDay, nNum, nN);

        for (int i = 0; i < nNum; i++) {
            if (pFDay[i].m_pfInd[c+2] != pFDay[i].m_pfInd[c+1])
                pFDay[i].m_pfInd[c] = (pFDay[i].m_pfInd[c+2] - pFDay[i].m_fClose) /
                                      (pFDay[i].m_pfInd[c+2] - pFDay[i].m_pfInd[c+1]) * 100;
            else if (i == 0)
                pFDay[i].m_pfInd[c] = 0;
            else
                pFDay[i].m_pfInd[c] = pFDay[i - 1].m_pfInd[c];
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInd_VR::CInd_VR() {
    _indParamNames[0] = "N";

    _indOutlineNames[0] = "VR";
}

void CInd_VR::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize==1);
    m_cExpSize = 1;

    if (pFDay == NULL || nNum <= 0) return;

    double* pfLV = new(std::nothrow) double[nNum];ZeroMemory(pfLV, nNum*sizeof(double));
    double* pfSV = new(std::nothrow) double[nNum];ZeroMemory(pfSV, nNum*sizeof(double));
    double* pfLV_SUM = new(std::nothrow) double[nNum];ZeroMemory(pfLV_SUM, nNum*sizeof(double));
    double* pfSV_SUM = new(std::nothrow) double[nNum];ZeroMemory(pfSV_SUM, nNum*sizeof(double));

    if (pfLV && pfSV && pfLV_SUM && pfSV_SUM)
    {
        int nN = m_psParam[0];
        if (nN<1)
            nN = 1;

        for (int i=1; i<nNum; i++)
        {
            if (pFDay[i].m_fClose > pFDay[i-1].m_fClose)
            {
                pfLV[i] = pFDay[i].m_fVolume;
                pfSV[i] = 0;
            }
            else
            {
                pfLV[i] = 0;
                pfSV[i] = pFDay[i].m_fVolume;
            }
        }

        int nFirst;
        Calc_SUM(pfLV_SUM, pfLV, nNum, nN, 1, nFirst);
        Calc_SUM(pfSV_SUM, pfSV, nNum, nN, 1, nFirst);

        m_pnFirst[0] = 1;

        for (int i=m_pnFirst[0]; i<nNum; i++)
        {
            if (pfSV_SUM[i]!=0)
                pFDay[i].m_pfInd[0] = pfLV_SUM[i]/pfSV_SUM[i]*100;
            else if (i==nN-1)
                pFDay[i].m_pfInd[0] = __NAN;
            else
                pFDay[i].m_pfInd[0] = __NAN;//pFDay[i-1].m_pfInd[0];
        }
    }

    if (pfLV)
        delete [] pfLV;
    if (pfSV)
        delete [] pfSV;
    if (pfLV_SUM)
        delete [] pfLV_SUM;
    if (pfSV_SUM)
        delete [] pfSV_SUM;
}
//////////////////////////////////////////////////////////////////////////
CInd_DMI::CInd_DMI() : CInd()
{
    
    _indParamNames[0] = "N";
    _indParamNames[1] = "M";

    _indOutlineNames[0] = "PDI";
    _indOutlineNames[1] = "MDI";
    _indOutlineNames[2] = "ADX";
    _indOutlineNames[3] = "ADXR";

    m_psParam[0] = 14;
    m_psParam[1] = 6;
}

void CInd_DMI::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 2);
    m_cExpSize = 4;

    if (pFDay == NULL || nNum <= 0) return;

    int nN = m_psParam[0];
    if (nN < 2) nN = 2;
    if (nN > 100) nN = 100;

    int nM = m_psParam[1];
    if (nM < 1) nM = 1;
    if (nM > 100) nM = 100;

    //if (nNum < nN) return;

    m_pnFirst[0] = m_pnFirst[1] = 1;
    m_pnFirst[2] = nM - 1;
    m_pnFirst[3] = m_pnFirst[2] + nM;

    for (int i = 0; i < nNum; i++)
    {
        if (i < nN)
        {
            pFDay[i].m_pfInd[4] = 0.0;
            for (int j=1; j<=i; j++)
            {
                double fABS1 = fabs(pFDay[j].m_fHigh-pFDay[j-1].m_fClose);
                double fABS2 = fabs(pFDay[j].m_fLow-pFDay[j-1].m_fClose);
                double fMax = Max(Max(pFDay[j].m_fHigh - pFDay[j].m_fLow, fABS1), fABS2);;
                pFDay[i].m_pfInd[4] += fMax;
            }
        }
        else
        {
            pFDay[i].m_pfInd[4] = 0.0;
            for (int j=i-nN+1; j<=i; j++)
            {
                double fABS1 = fabs(pFDay[j].m_fHigh-pFDay[j-1].m_fClose);
                double fABS2 = fabs(pFDay[j].m_fLow-pFDay[j-1].m_fClose);
                double fMax = Max(Max(pFDay[j].m_fHigh - pFDay[j].m_fLow, fABS1), fABS2);;
                pFDay[i].m_pfInd[4] += fMax;
            }
        }
    }
    

    pFDay[0].m_pfInd[2] = 0.0;
    pFDay[0].m_pfInd[3] = 0.0;
    for (int i=1; i<nNum; i++)
    {
        double fHD = pFDay[i].m_fHigh - pFDay[i-1].m_fHigh;
        double fLD = pFDay[i-1].m_fLow - pFDay[i].m_fLow;
        pFDay[i].m_pfInd[2] = (fHD > 0 && fHD > fLD) ? fHD : 0;
        pFDay[i].m_pfInd[3] = (fLD > 0 && fLD > fHD) ? fLD : 0;
    }

    for (int i = 0; i < nNum; i++)
    {
        if (i < nN)
        {
            pFDay[i].m_pfInd[0] = 0.0;
            pFDay[i].m_pfInd[1] = 0.0;
            for (int j=1; j<=i; j++)
            {
                pFDay[i].m_pfInd[0] += pFDay[j].m_pfInd[2];
                pFDay[i].m_pfInd[1] += pFDay[j].m_pfInd[3];
            }

            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[0] * 100 / pFDay[i].m_pfInd[4];
            pFDay[i].m_pfInd[1] = pFDay[i].m_pfInd[1]* 100 / pFDay[i].m_pfInd[4];
        }
        else
        {
            pFDay[i].m_pfInd[0] = 0.0;
            pFDay[i].m_pfInd[1] = 0.0;
            for (int j=i-nN+1; j<=i; j++)
            {
                pFDay[i].m_pfInd[0] += pFDay[j].m_pfInd[2];
                pFDay[i].m_pfInd[1] += pFDay[j].m_pfInd[3];
            }

            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[0] * 100 / pFDay[i].m_pfInd[4];
            pFDay[i].m_pfInd[1] = pFDay[i].m_pfInd[1]* 100 / pFDay[i].m_pfInd[4];
        }
    }
    
    // ADX: MA(ABS(MDI-PDI)/(MDI+PDI)*100,M);
    for (int i=0; i<nNum; i++) {
        pFDay[i].m_pfInd[3] = fabs(pFDay[i].m_pfInd[1] - pFDay[i].m_pfInd[0])/(pFDay[i].m_pfInd[1] + pFDay[i].m_pfInd[0]) * 100;
    }

    for (int i=0; i<nNum; i++)
    {
        if (i < nM - 1) {
            pFDay[i].m_pfInd[2] = __NAN;//pFDay[i].m_pfInd[3];
        } else
        {
            double sum = __NAN;
            for(int j = i-nM+1; j <= i; j++) {
                if (isinf(pFDay[j].m_pfInd[3]) || isnan(pFDay[j].m_pfInd[3])) {
                    continue;
                } else if (isnan(sum)) {
                    sum = 0;
                }
                sum += pFDay[j].m_pfInd[3];
            }
            pFDay[i].m_pfInd[2] = sum / nM;
        }
    }
    

    for (int i = 0; i < nNum; i++)
    {
        if (i < nM - 1)
            pFDay[i].m_pfInd[3] = __NAN;//pFDay[i].m_pfInd[2];
        else
            pFDay[i].m_pfInd[3] = (pFDay[i].m_pfInd[2] + pFDay[i-nM].m_pfInd[2]) / 2;
    }
    
    
}

//////////////////////////////////////////////////////////////////////////
CInd_DMA::CInd_DMA() : CInd()
{
    _indParamNames[0] = "LONG";
    _indParamNames[1] = "SHORT";
    _indParamNames[2] = "M";

    _indOutlineNames[0] = "DDA";
    _indOutlineNames[1] = "AMA";

    m_psParam[0] = 10;
    m_psParam[1] = 50;
    m_psParam[2] = 10;
}

void CInd_DMA::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 3);
    m_cExpSize = 2;

    if (pFDay == NULL || nNum <= 0) return;

    int nShort = m_psParam[0];
    if (nShort < 2) nShort = 2;
    if (nShort > 300) nShort = 300;

    int nLong = m_psParam[1];
    if (nLong < 10) nLong = 10;
    if (nLong > 300) nLong = 300;

    int nM = m_psParam[2];
    if (nM < 1) nM = 1;
    if (nM > 300) nM = 300;

    m_pnFirst[0] = nLong - 1;
    m_pnFirst[1] = nLong + nM -2;

    if (nNum < nLong) return;

    for (int i=0; i<nNum; i++)
    {
        if (i < nShort-1)
            pFDay[i].m_pfInd[0] = __NAN;//pFDay[i].m_fClose;
        else
        {
            pFDay[i].m_pfInd[0] = 0.0;
            for (int j=i-nShort+1; j<=i; j++)
            {
                pFDay[i].m_pfInd[0] += pFDay[j].m_fClose;
            }
            pFDay[i].m_pfInd[0] /= nShort;
        }
    }

    for (int i=0; i<nNum; i++)
    {
        if (i < nLong - 1)
            pFDay[i].m_pfInd[1] = __NAN;//pFDay[i].m_fClose;
        else
        {
            pFDay[i].m_pfInd[1] = 0.0;
            for (int j=i-nLong+1; j<=i; j++)
                pFDay[i].m_pfInd[1] += pFDay[j].m_fClose;
            pFDay[i].m_pfInd[1] /= nLong;
        }
    }

    for (int i=0; i<nNum; i++)
    {
        if (i < m_pnFirst[0])
            pFDay[i].m_pfInd[0] = __NAN;
        else
            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[0] - pFDay[i].m_pfInd[1];
    }

    for (int i=0; i<nNum; i++)
    {
        if (i < m_pnFirst[1])
            pFDay[i].m_pfInd[1] = __NAN;//pFDay[0].m_pfInd[0];
        else
        {
            pFDay[i].m_pfInd[1] = 0.0;
            for (int j=i-nM+1; j<=i; j++)
                pFDay[i].m_pfInd[1] += pFDay[j<0?0:j].m_pfInd[0];
            pFDay[i].m_pfInd[1] /= nM;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
CInd_TRIX::CInd_TRIX() : CInd()
{    
    _indParamNames[0] = "N";
    _indParamNames[1] = "M";

    _indOutlineNames[0] = "TRIX";
    _indOutlineNames[1] = "TRMA";

    m_psParam[0] = 12;
    m_psParam[1] = 20;
}
//三重指数平均线
void CInd_TRIX::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 2);
    m_cExpSize = 2;

    if (pFDay == NULL || nNum <= 1) return;

    int nN = m_psParam[0];
    if (nN < 3) nN = 3;
    if (nN > 100) nN = 100;

    int nM = m_psParam[1];
    if (nM < 1) nM = 1;
    if (nM > 100) nM = 100;

    m_pnFirst[0] = 1;
    m_pnFirst[1] = nM;
    
    pFDay[0].m_pfInd[1] = pFDay[0].m_fClose;
    for (int i=1; i<nNum; i++)
        pFDay[i].m_pfInd[1] = (pFDay[i-1].m_pfInd[1] *(nN-1)+pFDay[i].m_fClose*2)/(nN+1);

    pFDay[0].m_pfInd[0] = pFDay[0].m_pfInd[1];
    for (int i=1; i<nNum; i++)
        pFDay[i].m_pfInd[0] = (pFDay[i-1].m_pfInd[0] *(nN-1)+pFDay[i].m_pfInd[1]*2)/(nN+1);

    pFDay[0].m_pfInd[1] = pFDay[0].m_pfInd[0];
    for (int i=1; i<nNum; i++)
        pFDay[i].m_pfInd[1] = (pFDay[i-1].m_pfInd[1] *(nN-1)+pFDay[i].m_pfInd[0]*2)/(nN+1);

    pFDay[0].m_pfInd[0] = __NAN;//pFDay[0].m_pfInd[1];
    for (int i=1; i<nNum; i++)
        pFDay[i].m_pfInd[0] = (pFDay[i].m_pfInd[1] - pFDay[i-1].m_pfInd[1])/pFDay[i-1].m_pfInd[1]*100;

    for (int i=0; i<nNum; i++)
    {
        if (i < nM)
            pFDay[i].m_pfInd[1] = __NAN;//pFDay[i].m_pfInd[0];
        else
        {
            pFDay[i].m_pfInd[1] = 0.0;
            for (int j=i-nM+1; j<=i; j++)
                pFDay[i].m_pfInd[1] += pFDay[j].m_pfInd[0];
            pFDay[i].m_pfInd[1] /= nM;
        }
    }

}

//////////////////////////////////////////////////////////////////////////
CInd_BRAR::CInd_BRAR() : CInd()
{
    _indParamNames[0] = "N";

    _indOutlineNames[0] = "AR";
    _indOutlineNames[1] = "BR";

    m_psParam[0] = 26;
}
//情绪指标
void CInd_BRAR::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 1);
    m_cExpSize = 2;

    if (pFDay == NULL || nNum <= 0) return;

    int nN = m_psParam[0];
    if (nN < 2) nN = 2;
    if (nN > 300) nN = 300;

    //if (nNum < nN) return;

    //m_pnFirst[0] = nN-1;
    m_pnFirst[0] = 0;
    m_pnFirst[1] = m_pnFirst[0] + 1;

    for (int i=0; i<nNum; i++)
    {
        if (i < nN-1)
        {
            //pFDay[i].m_pfInd[0] = 0.0;
            pFDay[i].m_pfInd[2] = 0.0;//pFDay[i].m_fHigh - pFDay[i].m_fOpen;
            pFDay[i].m_pfInd[3] = 0.0;//pFDay[i].m_fOpen - pFDay[i].m_fLow;
            for (int j=0; j<=i; j++)
            {
                pFDay[i].m_pfInd[2] += (pFDay[j].m_fHigh - pFDay[j].m_fOpen);
                pFDay[i].m_pfInd[3] += (pFDay[j].m_fOpen - pFDay[j].m_fLow);
            }
            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[2] / pFDay[i].m_pfInd[3] * 100;
            
            if isinf(pFDay[i].m_pfInd[0]) {
                pFDay[i].m_pfInd[0] = __NAN;
            }
        }
        else
        {
            //pFDay[i].m_pfInd[0] = 0.0;
            pFDay[i].m_pfInd[2] = 0.0;
            pFDay[i].m_pfInd[3] = 0.0;
            for (int j=i-nN+1; j<=i; j++)
            {
                pFDay[i].m_pfInd[2] += (pFDay[j].m_fHigh - pFDay[j].m_fOpen);
                pFDay[i].m_pfInd[3] += (pFDay[j].m_fOpen - pFDay[j].m_fLow);
            }

            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[2] / pFDay[i].m_pfInd[3] * 100;
            if isinf(pFDay[i].m_pfInd[0]) {
                pFDay[i].m_pfInd[0] = __NAN;
            }
        }
    }

    for (int i=0; i<nNum; i++)
    {
        if (i < nN)
        {
            pFDay[i].m_pfInd[1] = 0.0;
            pFDay[i].m_pfInd[2] = 0.0;//Max(0, pFDay[i].m_fHigh - pFDay[i-1].m_fClose);
            pFDay[i].m_pfInd[3] = 0.0;//Max(0, pFDay[i-1].m_fClose - pFDay[i].m_fLow);
            for (int j=1; j<=i; j++)
            {
                pFDay[i].m_pfInd[2] += Max(0.0, pFDay[j].m_fHigh - pFDay[j-1].m_fClose);
                pFDay[i].m_pfInd[3] += Max(0.0, pFDay[j-1].m_fClose - pFDay[j].m_fLow);
            }
            if(i == 0)
                pFDay[i].m_pfInd[1] = __NAN;
            else
                pFDay[i].m_pfInd[1] = pFDay[i].m_pfInd[2] / pFDay[i].m_pfInd[3] * 100;
            
            if isinf(pFDay[i].m_pfInd[1]) {
                pFDay[i].m_pfInd[1] = __NAN;
            }

        }
        else
        {
            pFDay[i].m_pfInd[1] = 0.0;
            pFDay[i].m_pfInd[2] = 0.0;
            pFDay[i].m_pfInd[3] = 0.0;
            for (int j=i-nN+1; j<=i; j++)
            {
                pFDay[i].m_pfInd[2] += Max(0.0, pFDay[j].m_fHigh - pFDay[j-1].m_fClose);
                pFDay[i].m_pfInd[3] += Max(0.0, pFDay[j-1].m_fClose - pFDay[j].m_fLow);
            }

            pFDay[i].m_pfInd[1] = pFDay[i].m_pfInd[2] / pFDay[i].m_pfInd[3] * 100;
            
            if isinf(pFDay[i].m_pfInd[1]) {
                pFDay[i].m_pfInd[1] = __NAN;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
CInd_CR::CInd_CR() : CInd()
{
    _indParamNames[0] = "N";
    _indParamNames[1] = "M1";
    _indParamNames[2] = "M2";
    _indParamNames[3] = "M3";

    _indOutlineNames[0] = "CR";
    _indOutlineNames[1] = "MA1";
    _indOutlineNames[2] = "MA2";
    _indOutlineNames[3] = "MA3";

    m_psParam[0] = 26;
    m_psParam[1] = 5;
    m_psParam[2] = 10;
    m_psParam[3] = 20;
}

//TODO:
//int CInd_CR::GetOutlineCount() {
//    return 4;
//}

//带状能量线
void CInd_CR::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 4);
    m_cExpSize = 4;

    if (pFDay == NULL || nNum <= 0) return;

    int nN = m_psParam[0];
    if (nN < 5) nN = 5;
    if (nN > 300) nN = 300;

    int nM1 = m_psParam[1];
    if (nM1 < 1) nM1 = 1;
    if (nM1 > 100) nM1 = 100;

    int nM2 = m_psParam[2];
    if (nM2 < 1) nM2 = 1;
    if (nM2 > 100) nM2 = 100;

    int nM3 = m_psParam[3];
    if (nM3 < 1) nM3 = 1;
    if (nM3 > 100) nM3 = 100;

    //if (nNum <= nN) return;

    int nRefIndex1 = int(nM1/2.5+1);//3
    int nRefIndex2 = int(nM2/2.5+1);//5
    int nRefIndex3 = int(nM3/2.5+1);//9

//    m_pnFirst[0] = nN;//26
    m_pnFirst[0] = 1;//26
    m_pnFirst[1] = m_pnFirst[0] + nM1 + nRefIndex1 - 1;
    m_pnFirst[2] = m_pnFirst[0] + nM2 + nRefIndex2 - 1;
    m_pnFirst[3] = m_pnFirst[0] + nM3 + nRefIndex3 - 1;

    for (int i=0; i<nNum; i++)
        pFDay[i].m_pfInd[4] = (pFDay[i].m_fHigh + pFDay[i].m_fLow + pFDay[i].m_fClose) / 3;

    for (int i=0; i<nNum; i++)
    {
        if (i < nN)
        {
            pFDay[i].m_pfInd[0] = 0.0;
            pFDay[i].m_pfInd[2] = 0.0;
            pFDay[i].m_pfInd[3] = 0.0;
            for (int j=1; j<=i; j++)
            {
                pFDay[i].m_pfInd[2] += Max(0, pFDay[j].m_fHigh - pFDay[j-1].m_pfInd[4]);
                pFDay[i].m_pfInd[3] += Max(0, pFDay[j-1].m_pfInd[4] - pFDay[j].m_fLow);
            }
            if(i == 0)
                pFDay[i].m_pfInd[0] = __NAN;
            else {
                pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[2] / pFDay[i].m_pfInd[3] * 100;
                if (isinf(pFDay[i].m_pfInd[0])) {
                    pFDay[i].m_pfInd[0] = __NAN;
                }
            }
        }
        else
        {
            pFDay[i].m_pfInd[0] = 0.0;
            pFDay[i].m_pfInd[2] = 0.0;
            pFDay[i].m_pfInd[3] = 0.0;
            for (int j=i-nN+1; j<=i; j++)
            {
                pFDay[i].m_pfInd[2] += Max(0, pFDay[j].m_fHigh - pFDay[j-1].m_pfInd[4]);
                pFDay[i].m_pfInd[3] += Max(0, pFDay[j-1].m_pfInd[4] - pFDay[j].m_fLow);
            }

            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[2] / pFDay[i].m_pfInd[3] * 100;
        }
    }

    int nSplit = m_pnFirst[1] - nRefIndex1;
    if (nSplit < nM1) nSplit = nM1;
    for (int i=0; i<nNum; i++)
    {
        if (i < nSplit)
            pFDay[i].m_pfInd[4] = __NAN;
        else
        {
            double sum = __NAN;
            for (int j=i-nM1+1; j<=i; j++) {
                if (isinf(pFDay[j].m_pfInd[0]) || isnan(pFDay[j].m_pfInd[0])) {
                    continue;
                } else if (isnan(sum)) {
                    sum = 0;
                }
                sum += pFDay[j].m_pfInd[0];
            }
            pFDay[i].m_pfInd[4] = sum/nM1;
        }
    }

    nSplit = m_pnFirst[1];
    if (nSplit < nRefIndex1) nSplit = nRefIndex1;
    for (int i=0; i<nNum; i++)
    {
        if (i < nSplit)
            pFDay[i].m_pfInd[1] = __NAN;
        else
            pFDay[i].m_pfInd[1] = pFDay[i-nRefIndex1].m_pfInd[4];
    }

    nSplit = m_pnFirst[2] - nRefIndex2;
    if (nSplit < nM2) nSplit = nM2;
    for (int i=0; i<nNum; i++)
    {
        if (i < nSplit)
            pFDay[i].m_pfInd[4] = __NAN;
        else
        {
            double sum = __NAN;
            for (int j=i-nM2+1; j<=i; j++) {
                if (isinf(pFDay[j].m_pfInd[0]) || isnan(pFDay[j].m_pfInd[0])) {
                    continue;
                } else if (isnan(sum)) {
                    sum = 0;
                }
                sum += pFDay[j].m_pfInd[0];
            }
            pFDay[i].m_pfInd[4] = sum/nM2;
        }
    }

    nSplit = m_pnFirst[2];
    if (nSplit < nRefIndex2) nSplit = nRefIndex2;
    for (int i=0; i<nNum; i++)
    {
        if (i < nSplit)
            pFDay[i].m_pfInd[2] = __NAN;
        else
            pFDay[i].m_pfInd[2] = pFDay[i-nRefIndex2].m_pfInd[4];
    }

    nSplit = m_pnFirst[2] + nRefIndex2;
    if (nSplit < nM3) nSplit = nM3;
    for (int i=0; i<nNum; i++)
    {
        if (i < nSplit)
            pFDay[i].m_pfInd[4] = __NAN;
        else
        {
            double sum = __NAN;
            for (int j=i-nM3+1; j<=i; j++) {
                if (isinf(pFDay[j].m_pfInd[0]) || isnan(pFDay[j].m_pfInd[0])) {
                    continue;
                } else if (isnan(sum)) {
                    sum = 0;
                }
                sum += pFDay[j].m_pfInd[0];
            }
            pFDay[i].m_pfInd[4] = sum/nM3;
        }
    }

    nSplit = m_pnFirst[3];
    if (nSplit < nRefIndex3) nSplit = nRefIndex3;
    for (int i=0; i<nNum; i++)
    {
        if (i < nSplit)
            pFDay[i].m_pfInd[3] = __NAN;
        else
            pFDay[i].m_pfInd[3] = pFDay[i-nRefIndex3].m_pfInd[4];
    }
}

//////////////////////////////////////////////////////////////////////////
//累计能量线
CInd_OBV::CInd_OBV() : CInd()
{
    _indOutlineNames[0] = "OBV";
}

void CInd_OBV::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 0);
    m_cExpSize = 1;

    if (pFDay == NULL || nNum <= 0) return;

    m_pnFirst[0] = 1;

    for (int i=0; i<nNum; i++)
    {
        printf("Volume: %0.2f \n", pFDay[i].m_fVolume);
        pFDay[i].m_pfInd[1] = 0.0;
        if (i < 1)
            pFDay[i].m_pfInd[1] = 0.0;//pFDay[i].m_fVolume;
        else if (pFDay[i].m_fClose > pFDay[i-1].m_fClose)
            pFDay[i].m_pfInd[1] = pFDay[i].m_fVolume;
        else if (pFDay[i].m_fClose < pFDay[i-1].m_fClose)
            pFDay[i].m_pfInd[1] = -pFDay[i].m_fVolume;
        else
            pFDay[i].m_pfInd[1] = 0.0;
    }

    for (int i=0; i<nNum; i++)
    {
        if (i == 0)
            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[1];
        else if (i == 1) {
            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[1];
        } else {
            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[1] + pFDay[i-1].m_pfInd[0];
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//振动升降指标
CInd_ASI::CInd_ASI() {
    _indOutlineNames[0] = "ASI";
}

void CInd_ASI::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 0);
    m_cExpSize = 1;

    if (pFDay == NULL || nNum <= 0) return;

    m_pnFirst[0] = 1;

    for (int i=0; i<nNum; i++)
    {
        pFDay[i].m_pfInd[1] = 0.0;
        if (i > 0)
        {
            double fLC = pFDay[i-1].m_fClose;
            double fAA = fabs(pFDay[i].m_fHigh - fLC);
            double fBB = fabs(pFDay[i].m_fLow - fLC);
            double fCC = fabs(pFDay[i].m_fHigh - pFDay[i-1].m_fLow);
            double fDD = fabs(fLC - pFDay[i-1].m_fOpen);
            double fR = 0.0;
            if (fAA>fBB && fAA>fCC)
                fR = fAA + fBB/2 + fDD/4;
            else if (fBB>fCC && fBB>fAA)
                fR = fBB + fAA/2 + fDD/4;
            else
                fR = fCC + fDD/4;
            double fX = pFDay[i].m_fClose - fLC + (pFDay[i].m_fClose - pFDay[i].m_fOpen) / 2 + fLC - pFDay[i-1].m_fOpen;

            if(fR == 0 || Max(fAA,fBB) == 0)
                pFDay[i].m_pfInd[1] = 0;
            else
                pFDay[i].m_pfInd[1] = 16 * fX / fR * Max(fAA, fBB);
        }
    }

    for (int i=0; i<nNum; i++)
    {
        if (i == 0)
            pFDay[i].m_pfInd[0] = __NAN;//pFDay[i].m_pfInd[1];
        else if (i == 1) {
            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[1];
        } else {
            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[1] + pFDay[i-1].m_pfInd[0];
        }
    }
}

//////////////////////////////////////////////////////////////////////////
CInd_EMV::CInd_EMV() : CInd()
{
    _indParamNames[0] = "N";
    _indParamNames[1] = "N1";

    _indOutlineNames[0] = "EMV";
    _indOutlineNames[1] = "EMVA";

    m_psParam[0] = 14;
    m_psParam[1] = 9;
}
//简易波动指标
void CInd_EMV::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 2);
    m_cExpSize = 2;

    if (pFDay == NULL || nNum <= 0) return;

    int nN = m_psParam[0];
    if (nN < 1) nN = 1;
    if (nN > 100) nN = 100;

    int nM = m_psParam[1];
    if (nM < 1) nM = 1;
    if (nM > 100) nM = 100;

    m_pnFirst[0] = nN;
    m_pnFirst[1] = m_pnFirst[0] + nM - 1;

    if (nNum < nN) return;

    for (int i=0; i<nNum; i++)
    {
        pFDay[i].m_pfInd[3] = 0.0;
        if (i < 1) continue;

        double fA = (pFDay[i].m_fHigh+pFDay[i].m_fLow) / 2;
        double fB = (pFDay[i-1].m_fHigh+pFDay[i-1].m_fLow) / 2;
        double fMID = fA - fB;
        double fBRO = pFDay[i].m_fVolume / (pFDay[i].m_fHigh - pFDay[i].m_fLow);

        pFDay[i].m_pfInd[3] = fMID / fBRO;
    }

    for (int i=0; i<nNum; i++)
    {
        if (i < m_pnFirst[0])
            pFDay[i].m_pfInd[1] = 0.0;//pFDay[i].m_pfInd[3];
        else
        {
            pFDay[i].m_pfInd[1] = 0.0;
            for (int j=i-nN+1; j<=i; j++)
                pFDay[i].m_pfInd[1] += pFDay[j].m_pfInd[3];
            pFDay[i].m_pfInd[1] /= nN;
        }
    }

    pFDay[0].m_pfInd[2] = pFDay[0].m_fVolume;
    for (int i=1; i<nNum; i++)
        pFDay[i].m_pfInd[2] = (pFDay[i-1].m_pfInd[2]*(250-1)+pFDay[i].m_fVolume*2)/(250+1);

    for (int i=0; i<nNum; i++) {
        if (i < m_pnFirst[0]) {
            pFDay[i].m_pfInd[0] = __NAN;//pFDay[i].m_pfInd[0];
        } else {
            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[1] * pFDay[i].m_pfInd[2];
        }
    }
    
    for (int i=0; i<nNum; i++)
    {
        if (i < m_pnFirst[1])
            pFDay[i].m_pfInd[1] = __NAN;//pFDay[i].m_pfInd[0];
        else
        {
            pFDay[i].m_pfInd[1] = 0.0;
            for (int j=i-nM+1; j<=i; j++)
                pFDay[i].m_pfInd[1] += pFDay[j].m_pfInd[0];
            pFDay[i].m_pfInd[1] /= nM;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
CInd_CCI::CInd_CCI() : CInd()
{
    _indParamNames[0] = "N";

    _indOutlineNames[0] = "CCI";
    
    m_psParam[0] = 14;
}
//商品路径指标
void CInd_CCI::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 1);
    m_cExpSize = 1;

    if (pFDay == NULL || nNum <= 0) return;

    int nN = m_psParam[0];
    if (nN < 2) nN = 2;
    if (nN > 100) nN = 100;

    m_pnFirst[0] = nN-1;

    if (nNum < nN) return;

    for (int i=0; i<nNum; i++)
        pFDay[i].m_pfInd[3] = (pFDay[i].m_fHigh + pFDay[i].m_fLow + pFDay[i].m_fClose) / 3;

    for (int i=0; i<nNum; i++)
    {
        if (i < m_pnFirst[0])
            pFDay[i].m_pfInd[2] = 0.0;//pFDay[i].m_pfInd[3];
        else
        {
            for (int j=i-nN+1; j<=i; j++)
                pFDay[i].m_pfInd[2] += pFDay[j].m_pfInd[3];
            pFDay[i].m_pfInd[2] /= nN;
        }
    }


    for (int i=0; i<nNum; i++)
    {
        if (i < m_pnFirst[0])
            pFDay[i].m_pfInd[1] = 0;//pFDay[i].m_pfInd[0];
        else
        {
            pFDay[i].m_pfInd[1] = 0.0;
            for (int j=i-nN+1; j<=i; j++)
                pFDay[i].m_pfInd[1] += fabs(pFDay[j].m_pfInd[3] - pFDay[i].m_pfInd[2]);
            pFDay[i].m_pfInd[1] /= nN;
        }
    }

    for (int i=0; i<nNum; i++)
    {
        if (i < m_pnFirst[0]) {
            pFDay[i].m_pfInd[0] = __NAN;
        } else {
            pFDay[i].m_pfInd[0] = (pFDay[i].m_pfInd[3] - pFDay[i].m_pfInd[2]) / (0.015 * pFDay[i].m_pfInd[1]);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
CInd_ROC::CInd_ROC() : CInd()
{
    _indParamNames[0] = "N";
    _indParamNames[1] = "M";

    _indOutlineNames[0] = "ROC";
    _indOutlineNames[1] = "ROCMA";

    m_psParam[0] = 12;
    m_psParam[1] = 6;
}
//变动率指标
void CInd_ROC::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 2);
    m_cExpSize = 2;

    if (pFDay == NULL || nNum <= 0) return;

    int nN = m_psParam[0];
    if (nN < 1) nN = 1;
    if (nN > 100) nN = 100;

    int nM = m_psParam[1];
    if (nM < 1) nM = 1;
    if (nM > 50) nM = 50;

    m_pnFirst[0] = nN;
    m_pnFirst[1] = m_pnFirst[0] + nM - 1;

    if (nNum < nN) return;

    for (int i=0; i<nNum; i++)
    {
        if (i < m_pnFirst[0])
            pFDay[i].m_pfInd[0] = __NAN;
        else
            pFDay[i].m_pfInd[0] = (pFDay[i].m_fClose - pFDay[i-nN].m_fClose) / pFDay[i-nN].m_fClose * 100;
    }

    for (int i=0; i<nNum; i++)
    {
        if (i < m_pnFirst[1])
            pFDay[i].m_pfInd[1] = __NAN;//pFDay[i].m_pfInd[0];
        else
        {
            pFDay[i].m_pfInd[1] = 0.0;
            for (int j=i-nM+1; j<=i; j++)
                pFDay[i].m_pfInd[1] += pFDay[j].m_pfInd[0];
            pFDay[i].m_pfInd[1] /= nM;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
CInd_MTM::CInd_MTM() : CInd()
{
    _indParamNames[0] = "N";
    _indParamNames[1] = "N1";

    _indOutlineNames[0] = "MTM";
    _indOutlineNames[1] = "MTMMA";

    m_psParam[0] = 6;
    m_psParam[1] = 6;
}
//动量线
void CInd_MTM::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 2);
    m_cExpSize = 2;

    if (pFDay == NULL || nNum <= 0) return;

    int nN = m_psParam[0];
    if (nN < 1) nN = 1;
    if (nN > 100) nN = 100;

    int nM = m_psParam[1];
    if (nM < 1) nM = 1;
    if (nM > 100) nM = 100;

    m_pnFirst[0] = nN;
    m_pnFirst[1] = m_pnFirst[0] + nM - 1;

    if (nNum < nN) return;

    for (int i=0; i<nNum; i++)
    {
        if (i < m_pnFirst[0])
            pFDay[i].m_pfInd[0] = __NAN;
        else
            pFDay[i].m_pfInd[0] = pFDay[i].m_fClose - pFDay[i-nN].m_fClose;
    }

    for (int i=0; i<nNum; i++)
    {
        if (i < m_pnFirst[1]) {
            pFDay[i].m_pfInd[1] = __NAN;
        } else {
            pFDay[i].m_pfInd[1] = 0.0;
            for (int j=i-nM+1; j<=i; j++)
                pFDay[i].m_pfInd[1] += pFDay[j].m_pfInd[0];
            pFDay[i].m_pfInd[1] /= nM;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//心理线
CInd_PSY::CInd_PSY() : CInd()
{
    _indOutlineNames[0] = "PSY";
    
    m_psParam[0] = 12;
}

void CInd_PSY::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 1);
    m_cExpSize = 1;

    if (pFDay == NULL || nNum <= 0) return;

    int nN = m_psParam[0];
    if (nN < 1) nN = 1;
    if (nN > 100) nN = 100;

    m_pnFirst[0] = 1;

    //if (nNum < nN) return;

    for (int i=0; i<nNum; i++)
    {
        if (i < nN)
        {
            pFDay[i].m_pfInd[0] = 0.0;
            for (int j=1; j<=i; j++)
            {
                if (pFDay[j].m_fClose > pFDay[j-1].m_fClose)
                    pFDay[i].m_pfInd[0] += 1;
            }

            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[0] / nN * 100;
        }
        else
        {
            pFDay[i].m_pfInd[0] = 0.0;
            for (int j=i-nN+1; j<=i; j++)
            {
                if (pFDay[j].m_fClose > pFDay[j-1].m_fClose)
                    pFDay[i].m_pfInd[0] += 1;
            }

            pFDay[i].m_pfInd[0] = pFDay[i].m_pfInd[0] / nN * 100;
        }
    }
    
    pFDay[0].m_pfInd[0] = __NAN;

}

//////////////////////////////////////////////////////////////////////////
// DIS:=STDP(CLOSE,SD);
// MID:MA(CLOSE,SD),COLORFFAEC9;
// UPPER:MID+WIDTH*DIS,COLORFFC90E;
// LOWER:MID-WIDTH*DIS,COLOR0CAEE6;
CInd_BOLL::CInd_BOLL() : CInd()
{
    _indParamNames[0] = "N";
    _indParamNames[1] = "P";

    _indOutlineNames[0] = "MID";
    _indOutlineNames[1] = "UPPER";
    _indOutlineNames[2] = "LOWER";

    m_psParam[0] = 26;
    m_psParam[1] = 20;
}
//布林线
void CInd_BOLL::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 2);
    m_cExpSize = 3;

    if (pFDay == NULL || nNum <= 0) return;

    int nN = m_psParam[0];
    if (nN < 5) nN = 5;
    if (nN > 300) nN = 300;

    int nP = m_psParam[1];
    if (nP < 1) nP = 1;
    if (nP > 100) nP = 100;

    m_pnFirst[0] = m_pnFirst[1] = m_pnFirst[2] = nN - 1;
    if (nNum < nN) return;

    for (int i = 0; i < nNum; i++)
    {
        if (i < m_pnFirst[0])
            pFDay[i].m_pfInd[0] = __NAN;
        else
        {
            pFDay[i].m_pfInd[0] = 0.0;
            for (int j=i-nN+1; j<=i; j++)
                pFDay[i].m_pfInd[0] += pFDay[j].m_fClose;
            pFDay[i].m_pfInd[0] /= nN;
        }
    }

    for (int i=0; i<nNum; i++)
    {
        if (i < m_pnFirst[0])
            pFDay[i].m_pfInd[3] = __NAN;
        else
        {
            pFDay[i].m_pfInd[3] = 0.0;
            for (int j=i-nN+1; j<=i; j++)
                pFDay[i].m_pfInd[3] += pow(pFDay[j].m_fClose - pFDay[i].m_pfInd[0], 2);
            pFDay[i].m_pfInd[3] /= nN;
        }
    }

    for (int i = 0; i < nNum; i++)
    {
        if (i < m_pnFirst[0]) {
            pFDay[i].m_pfInd[0] = __NAN;
            pFDay[i].m_pfInd[1] = __NAN;
            pFDay[i].m_pfInd[2] = __NAN;
            pFDay[i].m_pfInd[3] = __NAN;
        } else {
            pFDay[i].m_pfInd[3] = sqrt(pFDay[i].m_pfInd[3] * nN / (nN - 1));
            pFDay[i].m_pfInd[1] = pFDay[i].m_pfInd[0] + nP / 10 * pFDay[i].m_pfInd[3];
            pFDay[i].m_pfInd[2] = pFDay[i].m_pfInd[0] - nP / 10 * pFDay[i].m_pfInd[3];
        }
    }
}



//////////////////////////////////////////////////////////////////////////
CInd_SAR::CInd_SAR() : CInd()
{
    _indParamNames[0] = "N";
    _indParamNames[1] = "STEP";
    _indParamNames[2] = "MAXP";

    m_psParam[0] = 10;
    m_psParam[1] = 2;
    m_psParam[2] = 20;
    
    _indOutlineNames[0] = "BB";

}

void CInd_SAR::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize == 3);
    m_cExpSize = 1;
    m_coloredIndIndex = 0;
    
    if (pFDay == NULL || nNum <= 0) return;

    int nN = m_psParam[0];//def 10
    if (nN < 1) nN = 1;
    if (nN > 100) nN = 100;

    int nStep = m_psParam[1];//def %2(0.02)
    if (nStep < 1) nStep = 1;
    if (nStep > 100) nStep = 100;

    int nMaxXP = m_psParam[2];//def 20%(0.2)
    if (nMaxXP < 5) nMaxXP = 5;
    if (nMaxXP > 100) nMaxXP = 100;

    m_pnFirst[0] = m_pnFirst[1] = nN;
    if (nNum <= nN) return;

    double fStep = double(nStep)/100.0;
    double fMax = double(nMaxXP)/100.0;

    BOOL bSARUp = TRUE;
    BOOL bLastUp = TRUE;
    double fLastMid = 0;
    int nUpDays = 0;

    // pFDay[i].m_pfInd[0] SAR(N, STEP, MAXP)
    // pFDay[i].m_pfInd[2] SARTURN(N, STEP, MAXP) 上升第一天1，下降第一天-1，其余0
    // pFDay[i].m_pfInd[1] SARSIGN(N, STEP, MAXP) 上升1，下降-1，上升下降第一天0
    for(int n = 0; n <= nN; n++)
    {
        double fMid = (pFDay[n].m_fHigh + pFDay[n].m_fLow)/2;
        if(n > 0)
        {
            if(fMid >= fLastMid)
            {
                nUpDays++;
                bLastUp = TRUE;
            }
            else
            {
                nUpDays--;
                bLastUp = FALSE;
            }
        }

        fLastMid = fMid;
    }

    if(nUpDays > 0)
        bSARUp = TRUE;
    else if(nUpDays < 0)
        bSARUp = FALSE;
    else
        bSARUp = bLastUp;

    double fAF = fStep;
    double fMaxHigh = 0;
    double fMinLow = 1e+10;
    for(int n = 0; n < nN; n++)
    {
        if(pFDay[n].m_fHigh > fMaxHigh)
            fMaxHigh = pFDay[n].m_fHigh;
        if(pFDay[n].m_fLow > 0 && pFDay[n].m_fLow < fMinLow)
            fMinLow = pFDay[n].m_fLow;
        
        pFDay[n].m_pfInd[0] = __NAN;
        pFDay[n].m_pfInd[1] = __NAN;
        pFDay[n].m_color = __NAN;
    }

    if(bSARUp)
    {
        pFDay[m_pnFirst[0]].m_pfInd[0] = fMinLow;
        pFDay[m_pnFirst[0]].m_pfInd[2] = 1;
    }
    else
    {
        pFDay[m_pnFirst[0]].m_pfInd[0] = fMaxHigh;
        pFDay[m_pnFirst[0]].m_pfInd[2] = -1;
    }
    
    pFDay[m_pnFirst[0]].m_pfInd[1] = 0;
    pFDay[m_pnFirst[0]].m_color = 0;

    for(int n = nN+1; n < nNum; n++)
    {
        fMaxHigh = 0;
        fMinLow = 1e+10;
        for(int m = n-nN; m < n; m++)
        {
            if(pFDay[m].m_fHigh > fMaxHigh)
                fMaxHigh = pFDay[m].m_fHigh;
            if(pFDay[m].m_fLow > 0 && pFDay[m].m_fLow < fMinLow)
                fMinLow = pFDay[m].m_fLow;
        }

        if(bSARUp)
        {
            if(pFDay[n-1].m_pfInd[2] == 0 && pFDay[n-1].m_pfInd[0] >= pFDay[n-1].m_fLow)	// turn
            {
                bSARUp = FALSE;

                pFDay[n].m_pfInd[0] = fMaxHigh;
                pFDay[n].m_pfInd[2] = -1;

                fAF = fStep;
            }
            else
            {
                if(pFDay[n].m_fHigh > fMaxHigh)
                {
                    if(fAF + fStep <= fMax)
                        fAF += fStep;
                }

                pFDay[n].m_pfInd[0] = pFDay[n-1].m_pfInd[0] + fAF*(fMaxHigh - pFDay[n-1].m_pfInd[0]);
                pFDay[n].m_pfInd[1] = 1;
            }
        }
        else	//SARDown
        {
            if(pFDay[n-1].m_pfInd[2] == 0 && pFDay[n-1].m_pfInd[0] <= pFDay[n-1].m_fHigh)	//turn
            {
                bSARUp = TRUE;

                pFDay[n].m_pfInd[0] = fMinLow;
                pFDay[n].m_pfInd[2] = 1;

                fAF = fStep;
            }
            else
            {
                if(pFDay[n].m_fLow < fMinLow)
                {
                    if(fAF + fStep <= fMax)
                        fAF += fStep;
                }

                pFDay[n].m_pfInd[0] = pFDay[n-1].m_pfInd[0] + fAF*(fMinLow - pFDay[n-1].m_pfInd[0]);
                pFDay[n].m_pfInd[1] = -1;
            }
        }
        
        pFDay[n].m_color = pFDay[n].m_pfInd[1];
    }
}


//////////////////////////////////////////////////////////////////////////
CInd_EMA::CInd_EMA()
{
    _indOutlineNames[0] = "EMA1";
    _indOutlineNames[1] = "EMA2";
    _indOutlineNames[2] = "EMA3";
    _indOutlineNames[3] = "EMA4";
    _indOutlineNames[4] = "EMA5";
    _indOutlineNames[5] = "EMA6";
    
    m_psParam[0] = 5;
    m_psParam[1] = 10;
    m_psParam[2] = 20;
    m_psParam[3] = 30;
    m_psParam[4] = 60;
    m_psParam[5] = 120;
}


// EMA（N）=2/（N+1）*（C-昨日EMA）+昨日EMA；
//m_psParam:均线值
void CInd_EMA::Calc(CFDayMobile* pFDay, int nNum)
{
    LogTrace("%s", __FUNCTION__);

    //ASSERT(m_cParamSize<=6);
    m_cExpSize = m_cParamSize;

    if (pFDay == NULL || nNum <= 0 ) return;

    for (int c=0; c<m_cParamSize; c++)
    {
        short sParam = m_psParam[c];
        if (sParam<1)
            sParam = 1;

        m_pnFirst[c] = int(sParam-1);  //设置前置K线根数

        for (int i=0; i<nNum; i++)
        {
            if (i<m_pnFirst[c])
                pFDay[i].m_pfInd[c] = pFDay[i].m_fClose;
            else if (sParam<2)
                pFDay[i].m_pfInd[c] = pFDay[i].m_fClose;
            else
            {
                pFDay[i].m_pfInd[c] = (pFDay[i-1].m_pfInd[c]*(sParam-1)+pFDay[i].m_fClose*2)/(sParam+1);
            }
        }
    }

}




CInd_SLOWKD::CInd_SLOWKD()
{
    _indParamNames[0] = "N";
    _indParamNames[1] = "M1";
    _indParamNames[2] = "M2";
    _indParamNames[3] = "M3";

    _indOutlineNames[0] = "K";
    _indOutlineNames[1] = "D";

    m_psParam[0] = 5;
    m_psParam[1] = 10;
    m_psParam[2] = 20;
    m_psParam[3] = 30;
    m_psParam[4] = 60;
    m_psParam[5] = 120;

}

//RSV:= (CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100;
//FASTK:=SMA(RSV,M1,1);
//K:SMA(FASTK,M2,1);
//D:SMA(K,M3,1)

void CInd_SLOWKD::Calc(CFDayMobile* pFDay, int nNum)
{

    //ASSERT(m_cParamSize==3);
    m_cExpSize = 2;

    if (pFDay == NULL || nNum <= 0) return;

    double* pfLLV = new(std::nothrow) double[nNum];ZeroMemory(pfLLV, nNum*sizeof(double));
    double* pfHHV = new(std::nothrow) double[nNum];ZeroMemory(pfHHV, nNum*sizeof(double));
    double* pfRSV = new(std::nothrow) double[nNum];ZeroMemory(pfRSV, nNum*sizeof(double));
    double* pfFASTK = new(std::nothrow) double[nNum];ZeroMemory(pfFASTK, nNum*sizeof(double));

    if (pfLLV && pfHHV && pfRSV)
    {
        int nN = m_psParam[0];
        int nM1 = m_psParam[1];
        int nM2 = m_psParam[2];
        int nM3 = m_psParam[3];


        Calc_LLV(pfLLV, pFDay, nNum, nN);
        Calc_HHV(pfHHV, pFDay, nNum, nN);

        int nFirst;
        if (nN-1>0)
            nFirst = nN-1;
        else
            nFirst = 0;
        for (int i=0; i<nNum; i++)
        {
            if (pfHHV[i]!=pfLLV[i])
                pfRSV[i] = (pFDay[i].m_fClose-pfLLV[i])/(pfHHV[i]-pfLLV[i])*100;
            else if (i==0)
                pfRSV[i] = 0;
            else
                pfRSV[i] = pfRSV[i-1];
        }

        double* pfFASTK = pfLLV;
        Calc_SMA(pfFASTK, pfRSV, nNum, nM1, 1, 0);


        m_pnFirst[0] = 0;
        double* pfK = pfLLV;
        Calc_SMA(pfK, pfFASTK, nNum, nM2, 1, 0);


        m_pnFirst[1] = 0;
        double* pfD = pfHHV;
        Calc_SMA(pfD, pfK, nNum, nM3, 1, 0);

        for (int i=0; i<nNum; i++)
        {
            pFDay[i].m_pfInd[0] = pfK[i];
            pFDay[i].m_pfInd[1] = pfD[i];
        }
    }

    if (pfLLV)
        delete [] pfLLV;
    if (pfHHV)
        delete [] pfHHV;
    if (pfRSV)
        delete [] pfRSV;
    if (pfFASTK)
        delete [] pfFASTK;

}



CInd_ENE::CInd_ENE()
{
    m_psParam[0] = 10;
    m_psParam[1] = 11;
    m_psParam[2] = 9;

}
//UPPER:(1+M1/100)*MA(CLOSE,N);
//LOWER:(1-M2/100)*MA(CLOSE,N);
//ENE:(UPPER+LOWER)/2;
//M1代表中轨基准线的上浮百分比，一般设为11（即11%）；
//M2代表中轨基准线的下跌百分比，一般设为9（即9%）。
//m_psParam[0]:N
//m_psParam[1]:M1
//m_psParam[2]:M2
void CInd_ENE::Calc(CFDayMobile* pFDay, int nNum)
{
    //ASSERT(m_cParamSize==3);
    m_cExpSize = 3;

    if (pFDay == NULL || nNum <= 0 || m_cExpSize !=3) return;

    //范围保护
    if (m_psParam[0] < 1 || m_psParam[0] >300)
        m_psParam[0] = 10;

    if (m_psParam[1] < 1 || m_psParam[1] >300)
        m_psParam[1] = 11;

    if (m_psParam[2] < 1 || m_psParam[2] >300)
        m_psParam[2] = 9;


    double* pfMA_Close = new(std::nothrow) double[nNum];ZeroMemory(pfMA_Close, nNum*sizeof(double));


    if (pfMA_Close)
    {
        Calc_MA(pfMA_Close, pFDay, nNum, m_psParam[0]);


        for (int i=0; i<nNum; i++)
        {
            pFDay[i].m_pfInd[0] = pfMA_Close[i];
            pFDay[i].m_pfInd[1] = (1+(double)m_psParam[1]/100)*pfMA_Close[i];
            pFDay[i].m_pfInd[2] = (1-(double)m_psParam[2]/100)*pfMA_Close[i];
        }
    }

    if (pfMA_Close)
        delete [] pfMA_Close;
}



CInd_BIAS::CInd_BIAS()
{
    m_psParam[0] = 6;
    m_psParam[1] = 12;
    m_psParam[2] = 30;
}


//BIAS1 : (CLOSE-MA(CLOSE,L1))/MA(CLOSE,L1)*100;
//BIAS2 : (CLOSE-MA(CLOSE,L2))/MA(CLOSE,L2)*100;
//BIAS3 : (CLOSE-MA(CLOSE,L3))/MA(CLOSE,L3)*100;
//m_psParam[0]: L1 (1, 300, 6);
//m_psParam[1]:	L2 (1, 300, 12);
//m_psParam[2]:	L3 (1, 300, 30);

void CInd_BIAS::Calc(CFDayMobile* pFDay, int nNum)
{

    //ASSERT(m_cParamSize==3);
    m_cExpSize = 3;

    if (pFDay == NULL || nNum <= 0 || m_cExpSize !=3) return;

    //参数范围保护
    if (m_psParam[0] < 1 || m_psParam[0] >300)
        m_psParam[0] = 6;

    if (m_psParam[1] < 1 || m_psParam[1] >300)
        m_psParam[1] = 12;

    if (m_psParam[2] < 1 || m_psParam[2] >300)
        m_psParam[2] = 30;


    double* pfMA_Close1 = new(std::nothrow) double[nNum];ZeroMemory(pfMA_Close1, nNum*sizeof(double));
    double* pfMA_Close2 = new(std::nothrow) double[nNum];ZeroMemory(pfMA_Close2, nNum*sizeof(double));
    double* pfMA_Close3 = new(std::nothrow) double[nNum];ZeroMemory(pfMA_Close3, nNum*sizeof(double));


    if (NULL != pfMA_Close1 && NULL != pfMA_Close2 && NULL != pfMA_Close3)
    {
        Calc_MA(pfMA_Close1, pFDay, nNum, m_psParam[0]);
        Calc_MA(pfMA_Close2, pFDay, nNum, m_psParam[0]);
        Calc_MA(pfMA_Close3, pFDay, nNum, m_psParam[0]);


        for (int i=0; i<nNum; i++)
        {
            if (0 == pfMA_Close1[i])
                pfMA_Close1[i] = 1;
            if (0 ==pfMA_Close2[i])
                pfMA_Close2[i] = 1;
            if (0 == pfMA_Close3[i])
                pfMA_Close3[i] = 1;

            pFDay[i].m_pfInd[0] = (pFDay[i].m_fClose-pfMA_Close1[i])/pfMA_Close1[i]*100;
            pFDay[i].m_pfInd[1] = (pFDay[i].m_fClose-pfMA_Close2[i])/pfMA_Close1[i]*100;
            pFDay[i].m_pfInd[2] = (pFDay[i].m_fClose-pfMA_Close3[i])/pfMA_Close1[i]*100;
        }
    }

    if (pfMA_Close1)
        delete [] pfMA_Close1;

    if (pfMA_Close2)
        delete [] pfMA_Close2;

    if (pfMA_Close3)
        delete [] pfMA_Close3;

}

//////////////////////////////////
// SWL:(EMA(CLOSE,5)*7+EMA(CLOSE,10)*3)/10;
// SWS:DMA(EMA(CLOSE,12),MAX(1,100*(SUM(VOL,5)/(3*CAPITAL))));
CInd_FSL::CInd_FSL() {
    _indOutlineNames[0] = "SWL";
    _indOutlineNames[1] = "SWS";
}

void CInd_FSL::Calc(CFDayMobile* pFDay, int nNum) {
    
    m_cExpSize = 2;
    m_pnFirst[0] = 0;
    m_pnFirst[1] = 0;
    
    if (pFDay == NULL || nNum <= 0) return;
    
    //{SWL
    pFDay[0].m_pfInd[2] = pFDay[0].m_fClose;
    pFDay[0].m_pfInd[3] = pFDay[0].m_fClose;
    for (int i=1; i<nNum; i++)
    {
        pFDay[i].m_pfInd[2] = (pFDay[i-1].m_pfInd[2] * (5 - 1) + pFDay[i].m_fClose*2) / (5 + 1);
        pFDay[i].m_pfInd[3] = (pFDay[i-1].m_pfInd[3] * (10 - 1) + pFDay[i].m_fClose*2) / (10 + 1);
    }
    
    for (int i=0; i<nNum; i++)
        pFDay[i].m_pfInd[0] = (pFDay[i].m_pfInd[2] * 7 + pFDay[i].m_pfInd[3] * 3) / 10;
    //}
    
    //{SWS
    pFDay[0].m_pfInd[2] = pFDay[0].m_fClose;
    for (int i=1; i<nNum; i++)
        pFDay[i].m_pfInd[2] = (pFDay[i-1].m_pfInd[2] * (12 - 1) + pFDay[i].m_fClose*2) / (12 + 1);
    
    for (int i=0; i<nNum; i++)
    {
        if (i < 5-1)
            pFDay[i].m_pfInd[3] = 0.0;
        else
        {
            pFDay[i].m_pfInd[3] = 0.0;
            for (int j=i-5+1; j<=i; j++)
                pFDay[i].m_pfInd[3] += pFDay[j].m_fVolume;
        }
        
        pFDay[i].m_pfInd[3] = Max(1, 100 * pFDay[i].m_pfInd[3] / (3 * pFDay[i].m_fLTG));
    }
    
    for (int i=0; i<nNum; i++)
    {
        if (i < 1)
            pFDay[i].m_pfInd[1] = pFDay[i].m_pfInd[0];
        else
            pFDay[i].m_pfInd[1] = pFDay[i].m_pfInd[3]*pFDay[i].m_pfInd[2]+(1-pFDay[i].m_pfInd[3])*pFDay[i-1].m_pfInd[1];
    }
    //}
};

