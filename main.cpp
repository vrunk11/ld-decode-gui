#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <ld_decode_gui/decode_gui.lay>
#include <CtrlCore/lay.h>

#include "ld_decode_utils.h"

using namespace Upp;

Thread thread1;

VectorMap<String, String> cfg = LoadIniFile("ld-decode-gui.cfg");

struct MyAppWindow : public Withdecode<TopWindow> {
	
    MyAppWindow() {
        CtrlLayout(*this, "LD DECODE");
        
        standard_list.Add("NTSC");
        standard_list.Add("NTSCJ");
        standard_list.Add("PAL");
        standard_list.SetIndex(0);
        
        option_start_list.Add("start");
        option_start_list.Add("seek");
        option_start_list.SetIndex(0);
        
        cfg.Get("default_opt_length") == "1" ? option_length.Set(1) : option_length.Set(0) ;
        
        btn_input_file << [=] {
        	FileSel file;
        	
        	if(DirectoryExists(cfg.Get("default_input_folder")))
       		{
       			file.ActiveDir(cfg.Get("default_input_folder"));
       		}
       		
       		file.ExecuteOpen();
       		if(file.GetFile(0) != "")
       		{
       			//PathFile = file.GetFile(0);
       			text_input_file.SetText(file.GetFile(0));
       			//PromptOK("you have selected (" + PathFile + ")");
       		}
        };
        
        btn_output_folder << [=] {
       		FileSel folder;
       		
       		if(DirectoryExists(cfg.Get("default_output_folder")))
       		{
       			folder.ActiveDir(cfg.Get("default_output_folder"));
       		}
       		
       		folder.ExecuteSelectDir();
       		if(folder.GetFile(0) != "")
       		{
       			//PathFolder = folder.GetFile(0);
       			text_output_folder.SetText(folder.GetFile(0));
       			//PromptOK("you have selected (" + PathFolder + ")");
       		}
       };
       
       btn_decode << [=] {
           if(FileExists(text_input_file.GetText().ToString()))
           {
               if(check_file_extention(cfg.Get("input_format"),GetFileExt(text_input_file.GetText().ToString())))
               {
	               if(DirectoryExists(text_output_folder.GetText().ToString()) && text_output_folder.GetText().ToString() != "")
	               {
			           //declaration
			           String command;
			           String opt_start;
			           String opt_length;
			           
			           String path_ld_analyse = cfg.Get("path_ld_analyse");
			           String file_name = text_input_file.GetText().ToString();
			           String path_output_folder = cfg.Get("default_output_folder");
			           
			           //assign
			           FileExists("ld-decode.py") ? (command = "ld-decode.py --") : (command = "ld-decode --");
			           option_start_list.GetValue() == "start" && numeric_start.GetText().ToString() != ""  ? opt_start = " -s " + numeric_start.GetText().ToString() + " " : opt_start = " -S " + numeric_start.GetText().ToString() + " ";
			           option_length.Get() && numeric_length.GetText().ToString() != "" ? opt_length = " -l " + numeric_length.GetText().ToString() + " " : " ";
			           
			           //composition command   
			           command += standard_list.GetValue().ToString() + opt_start + opt_length + file_name + " " + text_output_folder.GetText().ToString();
			           system("echo \"" + command + "\" & pause" );
			           command = "echo \"" + command + "\" & " + command + " & pause" ;
			           command += " && " + path_ld_analyse + " " + path_output_folder + "/" + file_name + ".tbc";
			           //run
			        	thread1.Start([=]{ system(command); });
	               }
	               else
	               {
	                   PromptOK("Output folder not found");
	               }
               }
               else
               {
                   PromptOK("File type not suported");
               }
           }
           else
           {
               PromptOK("Input file not found");
           }
       };
    }
};

void read_config_test()
{
	FileStream fConfig;
	String cfg_test;
	//load config files
    fConfig.Open("ld-decode-gui.cfg",Upp::BlockStream::READ);
    do 
    {
        cfg_test = fConfig.GetLine();
        PromptOK(cfg_test);
    }while(fConfig.IsEof() == false);
    fConfig.Close();
}

void load_config()
{
	VectorMap<String, String> cfg = LoadIniFile("ld-decode-gui.cfg");
}

GUI_APP_MAIN
{
	//read_config_test();
	if(!FileExists("ld-decode.py") && !FileExists("ld-decode")){PromptOK("ld-decode not found");}
    //run gui
    MyAppWindow().Run();
}

 