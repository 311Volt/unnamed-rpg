class BlockDB
{
    int BlockTypeIDTracker;
    int BlockProtoIDTracker;
    stringstream converter;
    
public:
    BlockDB(){BlockTypeIDTracker=0;BlockProtoIDTracker=0;}
    
    class BlockType
    {
    public:
        int id;
        string name;
        double grip;
        vector<ALLEGRO_SAMPLE*> stepsound;
        
        ALLEGRO_SAMPLE* GetRandomSample()
        {
            return stepsound[rand()%stepsound.size()];
        }
    };
    vector<BlockType> blockType;
    class BlockProto
    {
        int GetXYVar(int x, int y)
        {
            srand(y*x+x);
            return rand();
        }
    public:
        int id;
        string name;
        bool edge_effect;
        bool random_rotation;
        bool depth_effect;
        double height;
        vector<ALLEGRO_BITMAP*> texture;
        ALLEGRO_BITMAP* edge_texture_side;
        ALLEGRO_BITMAP* edge_texture_corner;
        string type;
        
        ALLEGRO_BITMAP* GetTextureFromPos(int x, int y)
        {
            //CriticalErrorNoClose("tex size"+IntToString(texture.size()));
            return texture[GetXYVar(x,y)%texture.size()];
        }
    };
    vector<BlockProto> blockProto;
    
    BlockProto* GetBlockByName(string name)
    {
        for(int i=0; i<blockProto.size(); i++)
        {
            if(blockProto[i].name == name)
                return &blockProto[i];
        }
        CriticalError1337("Couldn't find block name "+name);
        return NULL;
    }
    
    BlockProto* GetBlockByID(int id)
    {
        for(int i=0; i<blockProto.size(); i++)
        {
            if(blockProto[i].id == id)
                return &blockProto[i];
        }
        CriticalError1337("Couldn't find block id "+IntToString(id));
        return NULL;
    }
    
    BlockType* GetType(BlockProto* input)
    {
        for(int i=0; i<blockType.size(); i++)
        {
            if(input->type == blockType[i].name)
                return &blockType[i];
        }
        return NULL;
    }
    
    void ReadBlockTypeProto(string filename)
    {
        ifstream bt_proto_fs(filename.c_str());
        string search_tmp;
        ConfigFile bt_proto;
        bt_proto.LoadFile(filename.c_str());
        while(!bt_proto_fs.eof())
        {
            getline(bt_proto_fs, search_tmp);
            if(search_tmp[0] == '[' && search_tmp[search_tmp.size()-1] == ']')
            {
                string SectionName; for(int i=1; i<search_tmp.size()-1; i++) SectionName += search_tmp; 
                int t_var = bt_proto.GetSectionValue(SectionName, "StepSoundVariety");
                BlockType bt_tmp;
                bt_tmp.id = BlockTypeIDTracker;
                bt_tmp.name = SectionName;
                bt_tmp.grip = bt_proto.GetSectionValue(SectionName, "Grip");
                bt_tmp.stepsound.resize(t_var);
                for(int i=0; i<t_var; i++)
                {
                    converter.str("");
                    converter<<i+1;
                    string sndparam = "StepSound" + converter.str();
                    bt_tmp.stepsound[i] = sounds.GetSampleResource(bt_proto.GetSectionValueS(SectionName, sndparam));
                }
                blockType.push_back(bt_tmp);
                BlockTypeIDTracker++;
            }
        }
    }
    
    void ReadBlockProto(string filename)
    {
        ifstream b_proto_fs(filename.c_str());
        string search_tmp;
        ConfigFile b_proto;
        b_proto.LoadFile(filename.c_str());
        while(!b_proto_fs.eof())
        {
            search_tmp = "";
            getline(b_proto_fs, search_tmp);
            if(search_tmp[0] == '[' && search_tmp[search_tmp.size()-1] == ']')
            {
                string SectionName; for(int i=1; i<search_tmp.size()-1; i++) SectionName += search_tmp[i]; 
                int t_var = b_proto.GetSectionValue(SectionName, "TextureVariety");
                BlockProto b_tmp;
                b_tmp.id = BlockProtoIDTracker;
                b_tmp.name = SectionName;
                b_tmp.edge_effect = b_proto.GetSectionValue(SectionName, "EdgeEffect");
                b_tmp.depth_effect = b_proto.GetSectionValue(SectionName, "DepthEffect");
                b_tmp.random_rotation = b_proto.GetSectionValue(SectionName, "RandomRotation");
                
                if(b_tmp.edge_effect)
                {
                    b_tmp.edge_texture_side = textures.GetBitmapResource(b_proto.GetSectionValueS(SectionName, "EdgeTextureSide"));
                    b_tmp.edge_texture_corner = textures.GetBitmapResource(b_proto.GetSectionValueS(SectionName, "EdgeTextureCorner"));
                }
                if(b_tmp.depth_effect)
                {
                    b_tmp.height = (double)b_proto.GetSectionValue(SectionName, "Height")/1000.0;
                }
                b_tmp.type = b_proto.GetSectionValue(SectionName, "Material");
                b_tmp.texture.resize(t_var);
                for(int i=0; i<t_var; i++)
                {
                    converter.str("");
                    converter<<i+1;
                    string texparam = "Texture" + converter.str();
                    b_tmp.texture[i] = textures.GetBitmapResource(b_proto.GetSectionValueS(SectionName, texparam));
                }
                blockProto.push_back(b_tmp);
                BlockProtoIDTracker++;
            }
        }
    }
};

BlockDB blockDB;

void LoadBlockDB(string type_proto, string block_proto)
{
    fstream test;
    test.open(type_proto.c_str());
    if(!test.good())
        CriticalError1337("Error while loading block proto");
    test.close();
    test.open(block_proto.c_str());
    if(!test.good())
        CriticalError1337("error while loading block proto");
    test.close();
    blockDB.ReadBlockProto(block_proto);
    blockDB.ReadBlockTypeProto(type_proto);
}

