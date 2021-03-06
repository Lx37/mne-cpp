//=============================================================================================================
/**
* @file     pluginconnectorconnectionwidget.cpp
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     February, 2014
*
* @section  LICENSE
*
* Copyright (C) 2014, Christoph Dinh and Matti Hamalainen. All rights reserved.
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
* @brief    Contains the implementation of the PluginConnectorConnectionWidget class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "pluginconnectorconnectionwidget.h"
#include "pluginconnectorconnection.h"


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include <QGridLayout>
#include <QComboBox>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace MNEX;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================


PluginConnectorConnectionWidget::PluginConnectorConnectionWidget(PluginConnectorConnection* pPluginConnectorConnection, QWidget *parent)
: QWidget(parent)
, m_pPluginConnectorConnection(pPluginConnectorConnection)
{

    QWidget *rightFiller = new QWidget;
    rightFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QString sSender = m_pPluginConnectorConnection->getSender()->getName();
    QString sReceiver = m_pPluginConnectorConnection->getReceiver()->getName();

    m_pLabel = new QLabel(tr("Connector Connection: ")+sSender+" -> "+sReceiver, this);
    m_pLabel->setStyleSheet("border: 0px; font-size: 14px; font-weight: bold;");


    qint32 curRow = 0;

    QGridLayout *layout = new QGridLayout;
    layout->setMargin(5);
    layout->addWidget(m_pLabel,curRow,0,1,3);
    ++curRow;

    for(qint32 i = 0; i < m_pPluginConnectorConnection->getSender()->getOutputConnectors().size(); ++i)
    {
        QComboBox* m_pComboBox = new QComboBox(this);
        QLabel* m_pLabelOutput = new QLabel(m_pPluginConnectorConnection->getSender()->getOutputConnectors()[i]->getName(),this);
        layout->addWidget(m_pLabelOutput,curRow,0);

        ConnectorDataType t_senderConnectorDataType = PluginConnectorConnection::getDataType(m_pPluginConnectorConnection->getSender()->getOutputConnectors()[i]);

        m_pComboBox->addItem("----");

        for(qint32 j = 0; j < m_pPluginConnectorConnection->getReceiver()->getInputConnectors().size(); ++j)
        {
            ConnectorDataType t_receiverConnectorDataType = PluginConnectorConnection::getDataType(m_pPluginConnectorConnection->getReceiver()->getInputConnectors()[j]);
            if(t_senderConnectorDataType == t_receiverConnectorDataType)
                m_pComboBox->addItem(m_pPluginConnectorConnection->getReceiver()->getInputConnectors()[j]->getName());
        }

        layout->addWidget(m_pComboBox,curRow,1);
        ++curRow;
    }


    layout->addWidget(bottomFiller,curRow,0);
    ++curRow;

    layout->addWidget(rightFiller,1,3,curRow-1,1);

    this->setLayout(layout);
}
