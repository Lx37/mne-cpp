//=============================================================================================================
/**
* @file		rtmeasurementmanager.cpp
* @author	Christoph Dinh <christoph.dinh@live.de>;
* @version	1.0
* @date		October, 2010
*
* @section	LICENSE
*
* Copyright (C) 2010 Christoph Dinh. All rights reserved.
*
* No part of this program may be photocopied, reproduced,
* or translated to another program language without the
* prior written consent of the author.
*
*
* @brief	Contains the implementation of the RTMeasurementManager class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "rtmeasurementmanager.h"
#include <rtMeas/Measurement/IMeasurementprovider.h>
#include <rtMeas/Measurement/IMeasurementacceptor.h>

#include <rtMeas/Measurement/realtimesamplearray.h>
#include <rtMeas/Measurement/realtimemultisamplearray.h>
#include <rtMeas/Measurement/numeric.h>
//#include "Measurement/progressbar.h"
////#include "Measurement/alert.h"
//#include "Measurement/text.h"

#include <disp/realtimesamplearraywidget.h>
#include <disp/realtimemultisamplearraywidget.h>
#include <disp/numericwidget.h>
#include <disp/displaymanager.h>


#include <QDebug>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace RTMEASLIB;
using namespace RTDTMNGLIB;
using namespace DISPLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

RTMeasurementManager::RTMeasurementManager()
{

}


//*************************************************************************************************************

RTMeasurementManager::~RTMeasurementManager()
{

}


//*************************************************************************************************************

void RTMeasurementManager::addMeasurementProvider(IMeasurementProvider* pMSRPrv)
{
    qDebug() << "Number of Measurement Providers before: "<< s_hashMeasurementProvider.size() << "keys" << s_hashMeasurementProvider.keys();
    qDebug() << "Adding Measurement Provider: "<< pMSRPrv->getModule_ID();
    //check first if MeasurementProvider with id exists jet - and return existing first otherwise do the following stuff
    if(!s_hashMeasurementProvider.contains(pMSRPrv->getModule_ID()))
        s_hashMeasurementProvider.insert(pMSRPrv->getModule_ID(), pMSRPrv);

	qDebug() << "Number of Measurement Providers: "<< s_hashMeasurementProvider.size() << "keys" << s_hashMeasurementProvider.keys();
}


//*************************************************************************************************************

void RTMeasurementManager::addMeasurementAcceptor(IModule* pMSRAcc)//IMeasurementAcceptor* pMSRAcc)
{
	qDebug() << "Number of Measurement Acceptors before: "<< s_hashMeasurementAcceptor.size() << "keys" << s_hashMeasurementAcceptor.keys();
    qDebug() << "Adding Measurement Acceptor: "<< pMSRAcc->getModule_ID();
	//check first if MeasurementProvider with id exists jet - and return existing first otherwise do the following stuff
    if(!s_hashMeasurementAcceptor.contains(pMSRAcc->getModule_ID()))
        s_hashMeasurementAcceptor.insert(pMSRAcc->getModule_ID(), pMSRAcc);

	qDebug() << "Number of Measurement Acceptor: "<< s_hashMeasurementAcceptor.size() << "keys" << s_hashMeasurementAcceptor.keys();
}



//RTSA
//*************************************************************************************************************

void RTMeasurementManager::attachToRTSA(IObserver* pObserver) //attaching to all Measurements of all Measurement Providers
{
	QList<MDL_ID::Module_ID> mdl_idList = s_hashMeasurementProvider.keys();
	attachToRTSA(pObserver, mdl_idList);
}


//*************************************************************************************************************

void RTMeasurementManager::attachToRTSA(IObserver* pObserver, QList<MDL_ID::Module_ID> mdl_idList) //attaching to all Measurements of given Measurement Providers List
{
	QList<MSR_ID::Measurement_ID> msr_idList;

    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
    {
    	if(s_hashMeasurementProvider.contains(mdl_id))
    	{
            IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
			msr_idList += pMsrPvr->getProviderRTSA().keys();
    	}
    	else
    		qDebug() << "Error while attachToRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
    }
    attachToRTSA(pObserver, mdl_idList, msr_idList);
}


//*************************************************************************************************************

void RTMeasurementManager::attachToRTSA(IObserver* pObserver, QList<MDL_ID::Module_ID> mdl_idList, QList<MSR_ID::Measurement_ID> msr_idList) //attaching to given Measurements List of given Measurement Providers List
{
    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
    {
    	if(s_hashMeasurementProvider.contains(mdl_id))
    	{
            IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
		    foreach(MSR_ID::Measurement_ID msr_id, msr_idList)
		    {
		    	if(pMsrPvr->getProviderRTSA().contains(msr_id))
		    	{
		    		RealTimeSampleArray* pRTSA = pMsrPvr->getProviderRTSA().value(msr_id);
		    		pRTSA->attach(pObserver);
		    	}
		    	else
		    	{
		    		qDebug() << "Warning while attachToRTSA: MSR_ID::Measurement_ID " << msr_id << "is not part of measurement provider " << mdl_id << ".";
		    	}
		    }
    	}
    	else
    	{
    		qDebug() << "Error while attachToRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
    	}
    }
}


//*************************************************************************************************************

void RTMeasurementManager::attachWidgetsToRTSA(MDL_ID::Module_ID mdl_id, QTime* t) //attaching to given Measurements List of given Measurement Providers List
{
	qDebug() << "Number of Measurement Providers to attach Widget RTSA to: " << s_hashMeasurementProvider.size();
	if(s_hashMeasurementProvider.contains(mdl_id))
	{
        IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
		foreach(MSR_ID::Measurement_ID msr_id, pMsrPvr->getProviderRTSA().keys())
			attachWidgetToRTSA(mdl_id, msr_id, t);
	}
	else
		qDebug() << "Error while attachWidgetToRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
}


//*************************************************************************************************************

void RTMeasurementManager::attachWidgetToRTSA(MDL_ID::Module_ID mdl_id, MSR_ID::Measurement_ID msr_id, QTime* t) //attaching to given Measurements List of given Measurement Providers List
{
	if(s_hashMeasurementProvider.contains(mdl_id))
	{
        IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
		if(pMsrPvr->getProviderRTSA().contains(msr_id))
		{
			RealTimeSampleArray* pRTSA = pMsrPvr->getProviderRTSA().value(msr_id);
			if(pRTSA->isVisible())
			{
				IObserver* pRTSAWidget = dynamic_cast<IObserver*>(DisplayManager::addRealTimeSampleArrayWidget(pRTSA, 0, msr_id, t));
				pRTSA->attach(pRTSAWidget);
			}
		}
		else
		{
			qDebug() << "Warning while attachWidgetToRTSA: MSR_ID::Measurement_ID " << msr_id << "is not part of measurement provider " << mdl_id << ".";
		}
	}
	else
		qDebug() << "Error while attachWidgetToRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
}


//*************************************************************************************************************

void RTMeasurementManager::detachFromRTSA(IObserver* pObserver) //attaching to all Measurements of all Measurement Providers
{
	QList<MDL_ID::Module_ID> mdl_idList = s_hashMeasurementProvider.keys();

	detachFromRTSA(pObserver, mdl_idList);
}


//*************************************************************************************************************

void RTMeasurementManager::detachFromRTSA(IObserver* pObserver, QList<MDL_ID::Module_ID> mdl_idList) //attaching to all Measurements of given Measurement Providers List
{
	QList<MSR_ID::Measurement_ID> msr_idList;

    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
    {
    	if(s_hashMeasurementProvider.contains(mdl_id))
    	{
            IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
			msr_idList += pMsrPvr->getProviderRTSA().keys();
    	}
    	else
    		qDebug() << "Error while attachToRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
    }

    detachFromRTSA(pObserver, mdl_idList, msr_idList);
}


//*************************************************************************************************************

void RTMeasurementManager::detachFromRTSA(IObserver* pObserver, QList<MDL_ID::Module_ID> mdl_idList, QList<MSR_ID::Measurement_ID> msr_idList) //attaching to given Measurements List of given Measurement Providers List
{
    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
    {
    	if(s_hashMeasurementProvider.contains(mdl_id))
    	{
            IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
		    foreach(MSR_ID::Measurement_ID msr_id, msr_idList)
		    {
		    	if(pMsrPvr->getProviderRTSA().contains(msr_id))
		    	{
		    		RealTimeSampleArray* pRTSA = pMsrPvr->getProviderRTSA().value(msr_id);
		    		pRTSA->detach(pObserver);
		    	}
		    	else
		    	{
		    		qDebug() << "Warning while detachFromRTSA: MSR_ID::Measurement_ID " << msr_id << "is not part of measurement provider " << mdl_id << ".";
		    	}
		    }
    	}
    	else
    	{
    		qDebug() << "Error while detachFromRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
    	}
    }
}





































//RTMSA
//*************************************************************************************************************

void RTMeasurementManager::attachToRTMSA(IObserver* pObserver) //attaching to all Measurements of all Measurement Providers
{
    QList<MDL_ID::Module_ID> mdl_idList = s_hashMeasurementProvider.keys();
    attachToRTMSA(pObserver, mdl_idList);
}


//*************************************************************************************************************

void RTMeasurementManager::attachToRTMSA(IObserver* pObserver, QList<MDL_ID::Module_ID> mdl_idList) //attaching to all Measurements of given Measurement Providers List
{
    QList<MSR_ID::Measurement_ID> msr_idList;

    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
    {
        if(s_hashMeasurementProvider.contains(mdl_id))
        {
            IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
            msr_idList += pMsrPvr->getProviderRTMSA().keys();
        }
        else
            qDebug() << "Error while attachToRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
    }
    attachToRTMSA(pObserver, mdl_idList, msr_idList);
}


//*************************************************************************************************************

void RTMeasurementManager::attachToRTMSA(IObserver* pObserver, QList<MDL_ID::Module_ID> mdl_idList, QList<MSR_ID::Measurement_ID> msr_idList) //attaching to given Measurements List of given Measurement Providers List
{
    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
    {
        if(s_hashMeasurementProvider.contains(mdl_id))
        {
            IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
            foreach(MSR_ID::Measurement_ID msr_id, msr_idList)
            {
                if(pMsrPvr->getProviderRTMSA().contains(msr_id))
                {
                    RealTimeMultiSampleArray* pRTMSA = pMsrPvr->getProviderRTMSA().value(msr_id);
                    pRTMSA->attach(pObserver);
                }
                else
                {
                    qDebug() << "Warning while attachToRTSA: MSR_ID::Measurement_ID " << msr_id << "is not part of measurement provider " << mdl_id << ".";
                }
            }
        }
        else
        {
            qDebug() << "Error while attachToRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
        }
    }
}


//*************************************************************************************************************

void RTMeasurementManager::attachWidgetsToRTMSA(MDL_ID::Module_ID mdl_id, QTime* t) //attaching to given Measurements List of given Measurement Providers List
{
    qDebug() << "Number of Measurement Providers to attach Widget RTSA to: " << s_hashMeasurementProvider.size();
    if(s_hashMeasurementProvider.contains(mdl_id))
    {
        IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
        foreach(MSR_ID::Measurement_ID msr_id, pMsrPvr->getProviderRTMSA().keys())
            attachWidgetToRTMSA(mdl_id, msr_id, t);
    }
    else
        qDebug() << "Error while attachWidgetToRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
}


//*************************************************************************************************************

void RTMeasurementManager::attachWidgetToRTMSA(MDL_ID::Module_ID mdl_id, MSR_ID::Measurement_ID msr_id, QTime* t) //attaching to given Measurements List of given Measurement Providers List
{
    if(s_hashMeasurementProvider.contains(mdl_id))
    {
        IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
        if(pMsrPvr->getProviderRTMSA().contains(msr_id))
        {
            RealTimeMultiSampleArray* pRTMSA = pMsrPvr->getProviderRTMSA().value(msr_id);
            if(pRTMSA->isVisible())
            {
                IObserver* pRTMSAWidget = dynamic_cast<IObserver*>(DisplayManager::addRealTimeMultiSampleArrayWidget(pRTMSA, 0, msr_id, t));
                pRTMSA->attach(pRTMSAWidget);
            }
        }
        else
        {
            qDebug() << "Warning while attachWidgetToRTSA: MSR_ID::Measurement_ID " << msr_id << "is not part of measurement provider " << mdl_id << ".";
        }
    }
    else
        qDebug() << "Error while attachWidgetToRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
}


//*************************************************************************************************************

void RTMeasurementManager::detachFromRTMSA(IObserver* pObserver) //attaching to all Measurements of all Measurement Providers
{
    QList<MDL_ID::Module_ID> mdl_idList = s_hashMeasurementProvider.keys();

    detachFromRTMSA(pObserver, mdl_idList);
}


//*************************************************************************************************************

void RTMeasurementManager::detachFromRTMSA(IObserver* pObserver, QList<MDL_ID::Module_ID> mdl_idList) //attaching to all Measurements of given Measurement Providers List
{
    QList<MSR_ID::Measurement_ID> msr_idList;

    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
    {
        if(s_hashMeasurementProvider.contains(mdl_id))
        {
            IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
            msr_idList += pMsrPvr->getProviderRTMSA().keys();
        }
        else
            qDebug() << "Error while attachToRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
    }

    detachFromRTMSA(pObserver, mdl_idList, msr_idList);
}


//*************************************************************************************************************

void RTMeasurementManager::detachFromRTMSA(IObserver* pObserver, QList<MDL_ID::Module_ID> mdl_idList, QList<MSR_ID::Measurement_ID> msr_idList) //attaching to given Measurements List of given Measurement Providers List
{
    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
    {
        if(s_hashMeasurementProvider.contains(mdl_id))
        {
            IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
            foreach(MSR_ID::Measurement_ID msr_id, msr_idList)
            {
                if(pMsrPvr->getProviderRTMSA().contains(msr_id))
                {
                    RealTimeMultiSampleArray* pRTMSA = pMsrPvr->getProviderRTMSA().value(msr_id);
                    pRTMSA->detach(pObserver);
                }
                else
                {
                    qDebug() << "Warning while detachFromRTSA: MSR_ID::Measurement_ID " << msr_id << "is not part of measurement provider " << mdl_id << ".";
                }
            }
        }
        else
        {
            qDebug() << "Error while detachFromRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
        }
    }
}


























//*************************************************************************************************************

void RTMeasurementManager::attachToNumeric(IObserver* pObserver) //attaching to all Measurements of all Measurement Providers
{
    foreach(IMeasurementProvider* pMsrPvr, s_hashMeasurementProvider.values())
    {
        foreach(Numeric* pNum, pMsrPvr->getProviderNumeric().values())
        {
            pNum->attach(pObserver);
        }
    }
}


//*************************************************************************************************************

void RTMeasurementManager::attachToNumeric(IObserver* pObserver, QList<MDL_ID::Module_ID> mdl_idList) //attaching to all Measurements of given Measurement Providers List
{
    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
    {
        if(s_hashMeasurementProvider.contains(mdl_id))
        {
            IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
            foreach(Numeric* pNum, pMsrPvr->getProviderNumeric().values())
            {
                pNum->attach(pObserver);
            }
        }
        else
        {
            qDebug() << "Error while attachToNumeric: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
        }
    }
}


//*************************************************************************************************************

void RTMeasurementManager::attachToNumeric(IObserver* pObserver, QList<MDL_ID::Module_ID> mdl_idList, QList<MSR_ID::Measurement_ID> msr_idList) //attaching to given Measurements List of given Measurement Providers List
{
    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
    {
    	if(s_hashMeasurementProvider.contains(mdl_id))
    	{
    		IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
		    foreach(MSR_ID::Measurement_ID msr_id, msr_idList)
		    {
		    	if(pMsrPvr->getProviderNumeric().contains(msr_id))
		    	{
		    		Numeric* pNum = pMsrPvr->getProviderNumeric().value(msr_id);
					pNum->attach(pObserver);
		    	}
		    	else
		    	{
		    		qDebug() << "Warning while attachToNumeric: MSR_ID::Measurement_ID " << msr_id << "is not part of measurement provider " << mdl_id << ".";
		    	}
		    }
    	}
    	else
    	{
    		qDebug() << "Error while attachToNumeric: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
    	}
    }
}


//*************************************************************************************************************

void RTMeasurementManager::attachWidgetsToNumeric(MDL_ID::Module_ID mdl_id) //attaching to given Measurements List of given Measurement Providers List
{
	if(s_hashMeasurementProvider.contains(mdl_id))
	{
		IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
		foreach(MSR_ID::Measurement_ID msr_id, pMsrPvr->getProviderNumeric().keys())
			attachWidgetToNumeric(mdl_id, msr_id);
	}
	else
		qDebug() << "Error while attachWidgetToRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
}


//*************************************************************************************************************

void RTMeasurementManager::attachWidgetToNumeric(MDL_ID::Module_ID mdl_id, MSR_ID::Measurement_ID msr_id) //attaching to given Measurements List of given Measurement Providers List
{
	if(s_hashMeasurementProvider.contains(mdl_id))
	{
		IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
		if(pMsrPvr->getProviderNumeric().contains(msr_id))
		{
			Numeric* pNum = pMsrPvr->getProviderNumeric().value(msr_id);
			if(pNum->isVisible())
			{
				IObserver* pNumWidget = dynamic_cast<IObserver*>(DisplayManager::addNumericWidget(pNum, 0, msr_id));
				pNum->attach(pNumWidget);
			}
		}
		else
		{
			qDebug() << "Warning while attachWidgetToRTSA: MSR_ID::Measurement_ID " << msr_id << "is not part of measurement provider " << mdl_id << ".";
		}
	}
	else
		qDebug() << "Error while attachWidgetToRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
}

//*************************************************************************************************************

void RTMeasurementManager::detachFromNumeric(IObserver* pObserver) //attaching to all Measurements of all Measurement Providers
{
//	foreach(MeasurementProvider* pMsrPvr, s_hashMeasurementProvider.values())
//	{
//		foreach(Numeric* pNum, pMsrPvr->getProviderNumeric().values())
//		{
//			pNum->detach(pObserver);
//		}
//	}

	QList<MDL_ID::Module_ID> mdl_idList = s_hashMeasurementProvider.keys();

	detachFromNumeric(pObserver, mdl_idList);
}


//*************************************************************************************************************

void RTMeasurementManager::detachFromNumeric(IObserver* pObserver, QList<MDL_ID::Module_ID> mdl_idList) //attaching to all Measurements of given Measurement Providers List
{
    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
    {
    	if(s_hashMeasurementProvider.contains(mdl_id))
    	{
            IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);

			qDebug() << "RTMeasurementManager::detachFromNumeric: MSR_ID::Measurement_IDS: " << pMsrPvr->getProviderRTSA().keys();
			foreach(Numeric* pNum, pMsrPvr->getProviderNumeric().values())
			{
				pNum->detach(pObserver);
			}
    	}
    	else
    	{
    		qDebug() << "Error while detachFromNumeric: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
    	}
    }


    //ToDo ToDo
//	QList<MSR_ID::Measurement_ID> msr_idList;
//
//    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
//    {
//    	if(s_hashMeasurementProvider.contains(mdl_id))
//    	{
//			MeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
//			msr_idList += pMsrPvr->getProviderRTSA().keys();
//    	}
//    	else
//    		qDebug() << "Error while detachFromNumeric: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
//    }
//
//    detachFromNumeric(pObserver, mdl_idList, msr_idList);
}


//*************************************************************************************************************

void RTMeasurementManager::detachFromNumeric(IObserver* pObserver, QList<MDL_ID::Module_ID> mdl_idList, QList<MSR_ID::Measurement_ID> msr_idList) //attaching to given Measurements List of given Measurement Providers List
{
    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
    {
    	if(s_hashMeasurementProvider.contains(mdl_id))
    	{
    		IMeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
		    foreach(MSR_ID::Measurement_ID msr_id, msr_idList)
		    {
		    	if(pMsrPvr->getProviderNumeric().contains(msr_id))
		    	{
		    		Numeric* pNum = pMsrPvr->getProviderNumeric().value(msr_id);
					pNum->detach(pObserver);
		    	}
		    	else
		    	{
		    		qDebug() << "Warning while detachFromNumeric: MSR_ID::Measurement_ID " << msr_id << "is not part of measurement provider " << mdl_id << ".";
		    	}
		    }
    	}
    	else
    	{
    		qDebug() << "Error while detachFromNumeric: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
    	}
    }



//    foreach(MDL_ID::Module_ID mdl_id, mdl_idList)
//    {
//    	if(s_hashMeasurementProvider.contains(mdl_id))
//    	{
//			MeasurementProvider* pMsrPvr = s_hashMeasurementProvider.value(mdl_id);
//		    foreach(MSR_ID::Measurement_ID msr_id, msr_idList)
//		    {
//		    	if(pMsrPvr->getProviderRTSA().contains(msr_id))
//		    	{
//					Numeric* pNum = pMsrPvr->getProviderNumeric().value(msr_id);
//					pNum->detach(pObserver);
//		    	}
//		    	else
//		    	{
//		    		qDebug() << "Warning while detachFromRTSA: MSR_ID::Measurement_ID " << msr_id << "is not part of measurement provider " << mdl_id << ".";
//		    	}
//		    }
//    	}
//    	else
//    	{
//    		qDebug() << "Error while detachFromRTSA: MDL_ID::Module_ID " << mdl_id << "is no measurement provider.";
//    	}
//    }
}




//*************************************************************************************************************

void RTMeasurementManager::detachWidgets(QList<MDL_ID::Module_ID> mdl_idList)
{
	qDebug() << "RTMeasurementManager::detachWidgets(QList<MDL_ID::Module_ID> mdl_idList)";

	qDebug() << "DisplayManager::getRTSAWidgets() Size: " << DisplayManager::getRTSAWidgets().size();

	foreach(RealTimeSampleArrayWidget* pRTSAW, DisplayManager::getRTSAWidgets().values())
	{
    	IObserver* pRTSAWidget = dynamic_cast<IObserver*>(pRTSAW);

    	detachFromRTSA(pRTSAWidget, mdl_idList);
	}


	qDebug() << "DisplayManager::getNumericWidgets() Size: " << DisplayManager::getNumericWidgets().size();

	foreach(NumericWidget* pNUMW, DisplayManager::getNumericWidgets().values())
	{
    	IObserver* pNumWidget = dynamic_cast<IObserver*>(pNUMW);

		detachFromNumeric(pNumWidget, mdl_idList);
	}

}




//*************************************************************************************************************

void RTMeasurementManager::detachWidgets()
{
    qDebug() << "RTMeasurementManager::detachWidgets()";

    foreach(RealTimeSampleArrayWidget* pRTSAW, DisplayManager::getRTSAWidgets().values())
    {
        IObserver* pRTSAWidget = dynamic_cast<IObserver*>(pRTSAW);

        detachFromRTSA(pRTSAWidget);
    }

    foreach(NumericWidget* pNUMW, DisplayManager::getNumericWidgets().values())
    {
        IObserver* pNumWidget = dynamic_cast<IObserver*>(pNUMW);

        detachFromNumeric(pNumWidget);
    }

}


//*************************************************************************************************************

void RTMeasurementManager::clean()
{
    qDebug()<<"RTMeasurementManager::clean(): Cleaning up rt measurement manager";

//    foreach (IMeasurementProvider* value, s_hashMeasurementProvider)
//    		delete value;
//
//    foreach (IMeasurementAcceptor* value, s_hashMeasurementAcceptor)
//    		delete value;

    s_hashMeasurementProvider.clear();
    s_hashMeasurementAcceptor.clear();
}


//*************************************************************************************************************
//=============================================================================================================
// STATIC DEFINITIONS
//=============================================================================================================

QHash<MDL_ID::Module_ID, IMeasurementProvider*>    RTMeasurementManager::s_hashMeasurementProvider;
QHash<MDL_ID::Module_ID, IModule*>    RTMeasurementManager::s_hashMeasurementAcceptor;