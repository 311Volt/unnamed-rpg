
#define LIB_SOUNDS 1
#define LIB_IMAGES 2

#define LIB_EOF 1
#define LIB_OK 2

int fileSize(const char* path)
{
    fstream fs(path);
    fs.seekg(0, fs.end);
    return (int)fs.tellg();
}

class medialib
{
public:
    //zmienne///////////////////////////////////////////////////////////////////////////
    int libsize;
    int progress;
    int type;
    int rloaded;
    int liblistsize;

    //stringi/////////////////////////////////////////////////////////////////////////////
    string workPath;
    vector<string> filenames;
    vector<string> objnames;

    //fstream//////////////////////////////////////////////////////////////////////////////
    ifstream input;

    //klasy obs³uguj¹ce multimedia (Allegro)/////////////////////////////////////////////
    class mBitmapObject
    {
    public:
        ALLEGRO_BITMAP* bmp;
        string name;
    };
    class mSampleObject
    {
    public:
        ALLEGRO_SAMPLE* smp;
        string name;
    };

    //kolejne definicje
    vector<mBitmapObject> BitmapObject;
    vector<mSampleObject> SampleObject;

    //metody//////////////////////////////////////////////////////////////////////////////
    int LoadLib(string filename, string wPath, int wType)
    {

        //przypisania
        type = wType;
        rloaded = 0;
        progress = 0;
        libsize = 0;
        liblistsize = fileSize(filename.c_str());

        //zmienne tymczasowe
        workPath = wPath;
        string t0 = "";
        string t1 = "";
        char buf;
        bool phase = 0;

        //wczytywanie
        input.open(filename.c_str());
        if(!input.good()) return 0;
        for(int i=0; i<liblistsize; i++)
        {
            //wczytywanie znaku do bufora
            input.read(&buf,1);

            //obs³uga znaków specjalnych
            if(buf == '=')
            {
                phase = true;
                continue;
            }
            if(buf=='\n')
            {
                phase = false;
                t0 = wPath+t0;

                //przerzucenie do vectora, a nastêpnie zwolnienie stringów tymczasowych
                objnames.push_back(t1);
                filenames.push_back(t0);

                cout<<"Name:  "<<t1<<' ';
                cout<<"Path: "<<t0<<endl;

                ifstream test(t0.c_str());
                if(!test.good()) CriticalError_File404("BLAD: Nie znaleziono ", t0);
                test.close();

                t0 = "";
                t1 = "";
                continue;
            }

            //obs³uga nazw
            if(phase == 1) t0 = t0 + buf;
            if(phase == 0) t1 = t1 + buf;
        }

        //////////////////////////////////////////
        t0 = wPath+t0;
        string t0_tmp = "";
        for(int i=0; i<t0.size(); i++) if(t0[i] != ' ') t0_tmp += t0[i]; t0 = t0_tmp; //TERRIBLE HORRIBLE NO GOOD VERY BAD HACK
        objnames.push_back(t1);
        filenames.push_back(t0);
        cout<<"Name:  "<<t1<<' ';
        cout<<"Path: "<<t0<<endl;
        ifstream test(t0.c_str());
            if(!test.good()) CriticalError_File404("BLAD: Nie znaleziono ", t0);
        test.close();
        t0 = "";
        t1 = "";
        ///////////////////////////////////////

        for(int i=0; i<filenames.size(); i++)
            libsize += fileSize(filenames[i].c_str());
        cout<<"Libsize: "<<libsize/1024<<" KB\n";

        return 1;
    }

    int DestroyResources()
    {
        for(int i=0; i<BitmapObject.size(); i++)
            al_destroy_bitmap(BitmapObject[i].bmp);
        for(int i=0; i<SampleObject.size(); i++)
            al_destroy_sample(SampleObject[i].smp);

    }

    int NextResource()
    {
        //sprawdzenie
        if(rloaded > filenames.size()-1)
        {
            if(type == LIB_SOUNDS) al_reserve_samples(rloaded);
            return LIB_EOF;
        }

        if(type == LIB_IMAGES)
        {
            mBitmapObject tmp;
            if(objnames[rloaded][0] == 'B' && objnames[rloaded][1] == '_')
            {
                al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
                tmp.bmp = al_load_bitmap(filenames[rloaded].c_str());
            }
            else
            {
                al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP);
                tmp.bmp = al_load_bitmap(filenames[rloaded].c_str());
            }
            //aal_lock_bitmap(tmp.bmp, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
            tmp.name = objnames[rloaded];
            BitmapObject.push_back(tmp);
        }
        if(type == LIB_SOUNDS)
        {
            mSampleObject tmp;
            tmp.smp = al_load_sample(filenames[rloaded].c_str());
            tmp.name = objnames[rloaded];
            SampleObject.push_back(tmp);
        }
        cout<<filenames[rloaded]<<" loaded ("<<fileSize(filenames[rloaded].c_str())/1024<<" KB)\n";

        progress += fileSize(filenames[rloaded].c_str());
        rloaded++;

        return LIB_OK;
    }

    ALLEGRO_BITMAP* GetBitmapResource(string name)
    {
        for(int i=0; i<BitmapObject.size(); i++)
        {
            if(BitmapObject[i].name == name)
                return BitmapObject[i].bmp;
        }
        CriticalError1337("GetBitmapResource(): nie rozpoznano nazwy "+name+" ///");
    }

    ALLEGRO_SAMPLE* GetSampleResource(string name)
    {
        for(int i=0; i<SampleObject.size(); i++)
        {
            if(SampleObject[i].name == name)
                return SampleObject[i].smp;
        }
        CriticalError1337("GetSampleResource(): nie rozpoznano nazwy "+name+" ///");
    }

};
