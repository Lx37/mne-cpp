//=============================================================================================================
/**
* @file     inverseviewproducer.cpp
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>;
* @version  1.0
* @date     March, 2013
*
* @section  LICENSE
*
* Copyright (C) 2013, Christoph Dinh and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of the Massachusetts General Hospital nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MASSACHUSETTS GENERAL HOSPITAL BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    Implementation of the InverseViewProducer class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "inverseviewproducer.h"

#include "inverseview.h"

#include <QApplication>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace DISP3DLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

InverseViewProducer::InverseViewProducer(qint32 p_iFps, bool p_bLoop)
: m_bIsRunning(false)
, m_iFps(p_iFps)
, m_bLoop(p_bLoop)
, m_iT(0)
, m_iCurSampleStep(0)
, m_dGlobalMaximum(0)
, m_bBeep(false)
{
    float t_fT = 1.0 / (float)m_iFps;
    m_iT = (qint32)(t_fT*1000000);
}


//*************************************************************************************************************

InverseViewProducer::~InverseViewProducer()
{
}


//*************************************************************************************************************

void InverseViewProducer::pushSourceEstimate(MNESourceEstimate &p_sourceEstimate)
{
    mutex.lock();

    m_vecMaxActivation = p_sourceEstimate.data.rowwise().maxCoeff();
    m_dGlobalMaximum = m_vecMaxActivation.maxCoeff();

    float t_fTstep = p_sourceEstimate.tstep*1000000;

    qint32 t_iSampleStep = (qint32)ceil((float)m_iT)/t_fTstep;  //how many samples to skip
    qint32 t_iCurrentSample = m_iCurSampleStep;                 //take first or sample skip of previous push into account

    while(p_sourceEstimate.data.cols() > t_iCurrentSample)
    {
        m_vecStcs.append(p_sourceEstimate.data.col(t_iCurrentSample));
        m_vecTime.append(p_sourceEstimate.times(t_iCurrentSample));

        t_iCurrentSample += t_iSampleStep;
    }

    m_iCurSampleStep = t_iCurrentSample - p_sourceEstimate.data.cols();

    mutex.unlock();
}


//*************************************************************************************************************

void InverseViewProducer::stop()
{
    m_bIsRunning = false;

    // Stop threads
    QThread::terminate();
    QThread::wait();
}


//*************************************************************************************************************

void InverseViewProducer::run()
{
    qint32 simCount = 0;
    qint32 currentSample = 0;

    float t_fTimeOld = -1.0;

    m_bIsRunning = true;

    while(m_bIsRunning)
    {
        //LNdT hack
        mutex.lock();
        if(m_vecStcs.size() > 0)
        {
            //Loop
            if(m_bLoop)
            {
                currentSample = simCount%m_vecStcs.size();
                if (m_bBeep && ((t_fTimeOld < 0.0) && (m_vecTime[currentSample] >= 0.0)))
                {
                    QApplication::beep();
                    qDebug("beep");
                }
                t_fTimeOld = m_vecTime[currentSample];
                QSharedPointer<VectorXd> p_qVecCurrentActivation(new VectorXd(m_vecStcs[currentSample]));
                emit sourceEstimateSample(p_qVecCurrentActivation);

                ++simCount;
            }
            //Single view
            else
            {
                if (m_bBeep && ((t_fTimeOld < 0.0) && (m_vecTime[0] >= 0.0)))
                {
                    QApplication::beep();
                    qDebug("beep");
                }
                t_fTimeOld = m_vecTime[0];
                QSharedPointer<VectorXd> p_qVecCurrentActivation(new VectorXd(m_vecStcs[0]));
                emit sourceEstimateSample(p_qVecCurrentActivation);
                m_vecTime.pop_front();
                m_vecStcs.pop_front();
            }
        }


        mutex.unlock();

        usleep(m_iT);
    }
}
