//=============================================================================================================
/**
* @file     plotsignal.h
* @author   Florian Schlembach <florian.schlembach@tu-ilmenau.de>
*           Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>;
*           Jens Haueisen <jens.haueisen@tu-ilmenau.de>
* @version  1.0
* @date     January, 2014
*
* @section  LICENSE
*
* Copyright (C) 2014, Florian Schlembach, Christoph Dinh and Matti Hamalainen. All rights reserved.
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
* @brief    Implements the PlotSignalWidget of mne_browse_raw_qt
*
*/

#ifndef PLOTSIGNALWIDGET_H
#define PLOTSIGNALWIDGET_H

// ! INFORMATION !

// use this QWidget class as followed here:

//*****************************
//    //example for PlotSignalWidget
//    QFile t_rawFile("./MNE-sample-data/MEG/sample/sample_audvis_raw.fif");
//    FiffIO m_fiffIO(t_rawFile);

//    MatrixXd t_samples,t_times;
//    m_fiffIO.m_qlistRaw[0]->read_raw_segment_times(t_samples,t_times,100,102);
//    MatrixXd t_data;
//    t_data.resize(2,t_samples.cols());
//    t_data.row(0) = t_samples.row(0);
//    t_data.row(1) = t_times;
//    //generate PlotSignalWidget
//    PlotSignalWidget *plotSignalWidget = new PlotSignalWidget(t_data,this);
//    plotSignalWidget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//*****************************

//=============================================================================================================
// INCLUDES

//Qt
#include <QWidget>

//Eigen
#include <Eigen/Core>
#include <Eigen/SparseCore>

//=============================================================================================================
// DEFINE NAMESPACE

using namespace Eigen;

//=============================================================================================================

#include <QWidget>

class PlotSignalWidget : public QWidget
{
    Q_OBJECT
public:
    PlotSignalWidget(QWidget *parent = 0);
    PlotSignalWidget(MatrixXd data, QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    void createPlotPath(QPainterPath& path);
    void createGridPath(QPainterPath& path);

    QSize sizeHint() const;

    MatrixXd m_data;

    // Plots settings
    double m_dPlotHeight; /**< The height of the plot */

    // Scaling
    double m_dMaxValue; /**< Maximum value of the data to plot  */
    double m_dScaleY; /**< Maximum amplitude of plot (max is m_dPlotHeight/2) */
    double m_dDx; /**< pixel difference to the next sample*/

    //Look
    qint8 m_nhlines;
};

#endif // PLOTSIGNAL_H
