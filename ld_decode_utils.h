#ifndef _ld_decode_gui_ld_decode_utils_h_
#define _ld_decode_gui_ld_decode_utils_h_

using namespace Upp;

bool check_file_extention(String ext_list,String ext)
{
	Vector<String> list = Split(ext_list,',');
	
	for(int i=0; i < list.size();i++)
	{	
		if(ext == "." + list[i])
		{
			return true;
		}
	}
	return false;
}

void generate_config()
{
	if(!FileExists("ld-decode-gui.cfg"))
	{
		String cfg;

		cfg << "#the path of ld_analyse binary\n" << "path_ld_analyse=" << "\n\n";
		cfg << "#launch ld_analyse at the end of the decode (default = 0)\n" << "launch_ld_analyse=" << "0" << "\n\n";
		cfg << "#Default output folder in file selector\n" << "default_output_folder=" << "\n\n";
		cfg << "#Default input folder in file selector\n" << "default_input_folder=" << "\n\n";
		cfg << "#Enable the start option on startup\n" << "default_enable_start=" << "0" << "\n\n";
		cfg << "#select the standard on startup\n" << "default_standard=" << "NTSC" << "\n\n";
		cfg << "#select Start/Seek on startup\n" << "default_start=" << "Start" << "\n\n";
		cfg << "#select Length/Ending on startup\n" << "default_length=" << "Length" << "\n\n";
		cfg << "#Enable EFM option on startup\n" << "default_enable_efm=" << "1" << "\n\n";
		cfg << "#Enable Analog audio option on startup\n" << "default_enable_analog_audio=" << "1" << "\n\n";
		cfg << "#set the accepted input format\n" << "input_format=" << "s16,r16,u16,rf,r8,u8,lds,r30,raw.oga,ldf,wav,flac,vhs";

		if(!SaveFile("ld-decode-gui.cfg", cfg))
		{
    		Exclamation("Error saving configuration!");
		}
	}
}

String find_ending_timecode(int start,int end)
{
	int i = end - start;
	if(i <= 0)
	{
		return("1"); 
	}
	else
	{
		return(IntStr(i)); 
	}
}

String read_file_content(String inputFile)
{
	FileStream cfg_file;
        String file_content;
        
        cfg_file.Open(inputFile,BlockStream::READ);
        
        do 
		{
	        file_content += cfg_file.GetLine() + "\n";
    	}while(cfg_file.IsEof() == false);
       
        cfg_file.Close();
            	
    	return file_content;
}

#endif
