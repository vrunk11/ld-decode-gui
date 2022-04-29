#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <ld_decode_gui/decode_gui.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS icon
#define IMAGEFILE <ld_decode_gui/icon.iml>
#include <Draw/iml.h>

#include "ld_decode_utils.h"

#ifdef _WIN64
	#define PAUSE "pause"
#elif __linux
	#define PAUSE "read -n1"
#elif __APPLE__
	#define PAUSE "read -n1"
#endif

using namespace Upp;

Thread thread1;
VectorMap<String, String> cfg;

struct MyParametterWindow : public Withsetting<TopWindow> {

    MyParametterWindow() {
        CtrlLayout(*this, "LD DECODE SETTINGS");
        
    	//set the content of the file in the input text
        text_config.SetData(read_file_content("ld-decode-gui.cfg"));
        
        btn_save  << [=] {
	        if(!SaveFile("ld-decode-gui.cfg", text_config.GetData().ToString()))
			{
	    		Exclamation("Error saving configuration!");
			}
			else
			{
				PromptOK("Settings saved");
			}
        };
        
        btn_reset << [=] {
            if(PromptOKCancel("You are about to reset settings ! Do you want to proced ?"))
            {
	            //rewrite config file
	            generate_config();
	            
	            //set the content of the file in the input text
	        	text_config.SetData(read_file_content("ld-decode-gui.cfg"));    
            }
       };
    }
};

struct MyAppWindow : public Withdecode<TopWindow> {
	MenuBar menu;
	
    MyAppWindow() {
        AddFrame(menu);

        menu.Set([=](Bar& bar) {

            bar.Sub("Menu", [=](Bar& bar) {

                bar.Add("Settings",icon::settings, [=] {
					MyParametterWindow().Run();
                });

            });

        });
         
        CtrlLayout(*this, "LD DECODE");
        
        //set video standard
        standard_list.Add("NTSC");
        standard_list.Add("NTSCJ");
        standard_list.Add("PAL");
        
        if(ToUpper(cfg.Get("default_standard")) == "NTSC"){
        	standard_list.SetIndex(0);
        }
        else if(ToUpper(cfg.Get("default_standard")) == "NTSCJ"){
        	standard_list.SetIndex(1);
        }
        else if(ToUpper(cfg.Get("default_standard")) == "NTSC-J"){
            standard_list.SetIndex(1);
        }
        else if(ToUpper(cfg.Get("default_standard")) == "PAL"){
            standard_list.SetIndex(2);
        }
        
        cfg.Get("default_enable_efm") == "1" ? option_efm.Set(1) : option_efm.Set(0);
        cfg.Get("default_enable_analog_audio") == "1" ? option_analog_audio.Set(1) : option_analog_audio.Set(0);
        
        // Start-seek option list
        option_start_list.Add("Start");
        option_start_list.Add("Seek");
        option_start_list.SetIndex(0);
        
        //length
        option_length.Add("Length");
        option_length.Add("Ending");
        option_length.SetIndex(0);
        
        if(ToUpper(cfg.Get("default_start")) == "START"){
        	option_start_list.SetIndex(0);
        }
        else if(ToUpper(cfg.Get("default_start")) == "SEEK"){
        	option_start_list.SetIndex(1);
        }
        
        if(ToUpper(cfg.Get("default_length")) == "LENGTH"){
        	option_length.SetIndex(0);
        }
        else if(ToUpper(cfg.Get("default_length")) == "ENDING"){
        	option_length.SetIndex(1);
        }
        
        btn_input_file << [=] {
        	FileSel file;
        	
        	if(DirectoryExists(cfg.Get("default_input_folder")))
       		{
       			file.ActiveDir(cfg.Get("default_input_folder"));
       		}
       		
       		file.ExecuteOpen();
       		if(file.GetFile(0) != "")
       		{
       			label_file_name.SetText(GetFileName(file.GetFile(0)));
       			text_input_file.SetText(file.GetFile(0));
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
			           String command = "";
			           String opt_start = "";
			           String opt_length = "";
			           String opt_efm = "";
			           String opt_analog_audio = "";
			           
			           String path_ld_analyse = cfg.Get("path_ld_analyse");
			           String file_name = text_input_file.GetText().ToString();
			           String path_output_folder = cfg.Get("default_output_folder");
			           
			           //assign
			           FileExists("ld-decode.py") ? (command = "ld-decode.py --") : (command = "ld-decode --");
			           
			           if(box_option_start.Get())
			           {
			                numeric_start.GetText().ToString() != "" ? (option_start_list.GetValue() == "Start" ? opt_start = " -s " + numeric_start.GetText().ToString() + " " : opt_start = " -S " + numeric_start.GetText().ToString() + " ") : opt_start = "";
			            	numeric_length.GetText().ToString() != "" ? (option_length.GetValue() == "Length" ? opt_length = " -l " + numeric_length.GetText().ToString() + " " : (numeric_start.GetText().ToString() != "" ? opt_length = " -l " + find_ending_timecode(numeric_start.GetData(),numeric_length.GetData()) + " " :  opt_length = " -l " + numeric_length.GetText().ToString() + " ")) : " ";
			           }
			           !option_efm.Get() ? opt_efm = " --noEFM " : opt_efm = "";
			           !option_analog_audio.Get() ? opt_analog_audio = " --disable_analog_audio " : opt_analog_audio = "";
			           
			           //composition command   
			           command += standard_list.GetValue().ToString() + " " + opt_efm + opt_analog_audio + opt_start + opt_length + file_name + " " + text_output_folder.GetText().ToString();
			           system("echo \"" + command + "\" & " + PAUSE);
			           command = "echo \"" + command + "\" & " + command + " & " + PAUSE ;
			           
			           if(cfg.Get("launch_ld_analyse") == "1")
			           {
			               command += " && " + path_ld_analyse + " " + path_output_folder + "/" + file_name + ".tbc";
			           }
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

/*void read_config_test()
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
}*/

/*void load_config()
{
	VectorMap<String, String> cfg = LoadIniFile("ld-decode-gui.cfg");
}*/

GUI_APP_MAIN
{
	generate_config();
	//load config
	cfg = LoadIniFile("ld-decode-gui.cfg");
	//read_config_test();
	if(!FileExists("ld-decode.py") && !FileExists("ld-decode")){Exclamation("ld-decode not found");}
    //run gui
    MyAppWindow().Run();
}

 