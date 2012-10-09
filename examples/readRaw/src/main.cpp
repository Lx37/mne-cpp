//=============================================================================================================
/**
* @file     main.cpp
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti H�m�l�inen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     July, 2012
*
* @section  LICENSE
*
* Copyright (C) 2012, Christoph Dinh and Matti Hamalainen. All rights reserved.
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
* @brief    Implements the main() application function.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include <iostream>
#include <vector>
#include <math.h>


#include "../../../MNE/fiff/fiff.h"
#include "../../../MNE/mne/mne.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtCore/QCoreApplication>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace FIFFLIB;
using namespace MNELIB;


//*************************************************************************************************************
//=============================================================================================================
// MAIN
//=============================================================================================================

//=============================================================================================================
/**
* The function main marks the entry point of the program.
* By default, main has the storage class extern.
*
* @param [in] argc (argument count) is an integer that indicates how many arguments were entered on the command line when the program was started.
* @param [in] argv (argument vector) is an array of pointers to arrays of character objects. The array objects are null-terminated strings, representing the arguments that were entered on the command line when the program was started.
* @return the value that was set to exit() (which is 0 if exit() is called via quit()).
*/
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString t_sFile = "./MNE-sample-data/MEG/sample/sample_audvis_raw.fif";

    float from = 42.956;
    float to = 320.670;

    bool in_samples = false;

    bool keep_comp = true;

    //
    //   Setup for reading the raw data
    //
    FiffRawData* raw = NULL;
    if(!FiffFile::setup_read_raw(t_sFile, raw))
    {
        printf("Error during fiff setup raw read");
        return 0;
    }
    //
    //   Set up pick list: MEG + STI 014 - bad channels
    //
    //
    QStringList include;
    include << "STI 014";
    bool want_meg   = true;
    bool want_eeg   = false;
    bool want_stim  = false;

    MatrixXi picks = Fiff::pick_types(raw->info, want_meg, want_eeg, want_stim, include, raw->info->bads);


    //
    //   Set up projection
    //
    qint32 k = 0;
    if (raw->info->projs.size() == 0)
        printf("No projector specified for these data\n");
    else
    {
        //
        //   Activate the projection items
        //
        for (k = 0; k < raw->info->projs.size(); ++k)
            raw->info->projs[k]->active = true;

        printf("%d projection items activated\n",raw->info->projs.size());
        //
        //   Create the projector
        //
        fiff_int_t nproj = MNE::make_projector_info(raw->info, raw->proj);


//        qDebug() << raw->proj.data->data.rows();
//        qDebug() << raw->proj.data->data.cols();
//        std::cout << "proj: \n" << raw->proj.data->data.block(0,0,10,10);

        if (nproj == 0)
        {
            printf("The projection vectors do not apply to these channels\n");
        }
        else
        {
            printf("Created an SSP operator (subspace dimension = %d)\n",nproj);
        }
    }

    //
    //   Set up the CTF compensator
    //
    qint32 current_comp = MNE::get_current_comp(raw->info);
    qint32 dest_comp = -1;

    if (current_comp > 0)
        printf("Current compensation grade : %d\n",current_comp);

    if (keep_comp)
        dest_comp = current_comp;

    if (current_comp != dest_comp)
    {
//        try
//            raw.comp = mne_make_compensator(raw.info,current_comp,dest_comp);
//            raw.info.chs  = mne_set_current_comp(raw.info.chs,dest_comp);
//            printf("Appropriate compensator added to change to grade %d.\n",dest_comp);
//        catch
//            error(me,'%s',mne_omit_first_line(lasterr));
//        end
    }
    //
    //   Read a data segment
    //   times output argument is optional
    //
//    try

    bool readSuccessful = false;
    MatrixXf* data = NULL;
    MatrixXf* times = NULL;
    if (in_samples)
        readSuccessful = raw->read_raw_segment(data, times, (qint32)from, (qint32)to, picks);
    else
        readSuccessful = raw->read_raw_segment_times(data, times, from, to, picks);

    if (!readSuccessful)
    {
        printf("Could not read raw segment.\n");
        delete raw;
        return -1;
    }

    printf("Read %d samples.\n",data->cols());


    std::cout << data->block(0,0,10,10);


    delete raw;

    return a.exec();
}

//*************************************************************************************************************
//=============================================================================================================
// STATIC DEFINITIONS
//=============================================================================================================

