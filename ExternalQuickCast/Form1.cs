using IniParser;
using IniParser.Model;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ExternalQuickCast
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }


        string Num1KeyTextText = string.Empty;
        string Num2KeyTextText = string.Empty;
        string Num4KeyTextText = string.Empty;
        string Num5KeyTextText = string.Empty;
        string Num7KeyTextText = string.Empty;
        string Num8KeyTextText = string.Empty;

        string QKeyTextText = string.Empty;
        string WKeyTextText = string.Empty;
        string EKeyTextText = string.Empty;
        string RKeyTextText = string.Empty;
        string TKeyTextText = string.Empty;
        string YKeyTextText = string.Empty;

        string ConfigFileName = "conf.ini";

        private void Form1_Load(object sender, EventArgs e)
        {
            LoadConfigFile();
        }



        public void LoadConfigFile()
        {
            QKeyText.Text =
            WKeyText.Text =
            EKeyText.Text =
            RKeyText.Text =
            TKeyText.Text =
            YKeyText.Text =
            Num7KeyText.Text =
            Num8KeyText.Text =
            Num4KeyText.Text =
            Num5KeyText.Text =
            Num1KeyText.Text =
            Num2KeyText.Text =
          QKeyTextText =
            WKeyTextText =
            EKeyTextText =
            RKeyTextText =
            TKeyTextText =
            YKeyTextText =
            Num7KeyTextText =
            Num8KeyTextText =
            Num4KeyTextText =
            Num5KeyTextText =
            Num1KeyTextText =
            Num2KeyTextText =
            "";


            // Создать парсер INI файлов
            FileIniDataParser parser = new FileIniDataParser();
            // Создать хранилище INI данных 
            IniData data = new IniData();
            // Загрузить в data файл LauncherConfig.ini
            try
            {
                data = parser.ReadFile(ConfigFileName);
            }
            catch
            {

            }
            try
            {
                if (!data.Sections.ContainsSection("QUICKCAST"))
                {
                    data.Sections.Add(new SectionData("QUICKCAST"));
                }



                QKeyTextText = data["QUICKCAST"]["Q"];
                WKeyTextText = data["QUICKCAST"]["W"];
                EKeyTextText = data["QUICKCAST"]["E"];
                RKeyTextText = data["QUICKCAST"]["R"];
                TKeyTextText = data["QUICKCAST"]["T"];
                 YKeyTextText = data["QUICKCAST"]["Y"];
                Num1KeyTextText = data["QUICKCAST"]["Num1"];
                Num2KeyTextText = data["QUICKCAST"]["Num2"];
                Num4KeyTextText = data["QUICKCAST"]["Num4"];
                Num5KeyTextText = data["QUICKCAST"]["Num5"];
                Num7KeyTextText = data["QUICKCAST"]["Num7"];
                Num8KeyTextText = data["QUICKCAST"]["Num8"];




                QKeyText.Text = KeyHelper.MyKeyToStr(
                                    uint.Parse(QKeyTextText.Length > 0 ?
                                   QKeyTextText : "0")
                                    );
                WKeyText.Text = KeyHelper.MyKeyToStr(
                                    uint.Parse(WKeyTextText.Length > 0 ?
                                   WKeyTextText : "0")
                                    );
                EKeyText.Text = KeyHelper.MyKeyToStr(
                                    uint.Parse(EKeyTextText.Length > 0 ?
                                   EKeyTextText : "0")
                                    );
                RKeyText.Text = KeyHelper.MyKeyToStr(
                                    uint.Parse(RKeyTextText.Length > 0 ?
                                   RKeyTextText : "0")
                                    );
                TKeyText.Text = KeyHelper.MyKeyToStr(
                                    uint.Parse(TKeyTextText.Length > 0 ?
                                   TKeyTextText : "0")
                                    );
                YKeyText.Text = KeyHelper.MyKeyToStr(
                                    uint.Parse(YKeyTextText.Length > 0 ?
                                  YKeyTextText : "0")
                                    );
             
                Num1KeyText.Text = KeyHelper.MyKeyToStr(
                                    uint.Parse(Num1KeyTextText.Length > 0 ?
                                     Num1KeyTextText : "0")
                                    );


                Num2KeyText.Text = KeyHelper.MyKeyToStr(
                                    uint.Parse(Num2KeyTextText.Length > 0 ?
                                     Num2KeyTextText : "0")
                                    );
                Num4KeyText.Text = KeyHelper.MyKeyToStr(
                                    uint.Parse(Num4KeyTextText.Length > 0 ?
                                     Num4KeyTextText : "0")
                                    );
                Num5KeyText.Text = KeyHelper.MyKeyToStr(
                                    uint.Parse(Num5KeyTextText.Length > 0 ?
                                     Num5KeyTextText : "0")
                                    );
                Num7KeyText.Text = KeyHelper.MyKeyToStr(
                                    uint.Parse(Num7KeyTextText.Length > 0 ?
                                     Num7KeyTextText : "0")
                                    );
                Num8KeyText.Text = KeyHelper.MyKeyToStr(
                                    uint.Parse(Num8KeyTextText.Length > 0 ?
                                     Num8KeyTextText : "0")
                                    );


             

            }
            catch
            {
                UpdateConfigFile();
            }

        }

        public void UpdateConfigFile()
        {
            // Создать парсер INI файлов
            FileIniDataParser parser = new FileIniDataParser();
            // Создать хранилище INI данных 
            IniData data = new IniData();
            try
            {
                data = parser.ReadFile(ConfigFileName);
            }
            catch
            {

            }
            try
            {
                if (!data.Sections.ContainsSection("QUICKCAST"))
                    data.Sections.Add(new SectionData("QUICKCAST"));

            }
            catch
            {

            }


            try
            {
                data["QUICKCAST"]["Q"] = QKeyTextText;
                data["QUICKCAST"]["W"] = WKeyTextText;
                data["QUICKCAST"]["E"] = EKeyTextText;
                data["QUICKCAST"]["R"] = RKeyTextText;
                data["QUICKCAST"]["T"] = TKeyTextText;
                data["QUICKCAST"]["Y"] = YKeyTextText;

                data["QUICKCAST"]["Num1"] = Num1KeyTextText;
                data["QUICKCAST"]["Num2"] = Num2KeyTextText;
                data["QUICKCAST"]["Num4"] = Num4KeyTextText;
                data["QUICKCAST"]["Num5"] = Num5KeyTextText;
                data["QUICKCAST"]["Num7"] = Num7KeyTextText;
                data["QUICKCAST"]["Num8"] = Num8KeyTextText;


              

                // Сохранение в файл

                parser.WriteFile(ConfigFileName, data, Encoding.UTF8);

            }
            catch
            {

            }
        }


        private void VKeyText_KeyDown(object sender, KeyEventArgs e)
        {
            TextBox _sender = sender as TextBox;
            _sender.Text = KeyHelper.KeyAllKeys(2);

            if (_sender == QKeyText)
                QKeyTextText = KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == WKeyText)
                WKeyTextText = KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == EKeyText)
                EKeyTextText = KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == RKeyText)
                RKeyTextText = KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == TKeyText)
                TKeyTextText = KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == YKeyText)
                YKeyTextText = KeyHelper.KeyAllKeys_code(2).ToString();

            if (_sender == Num1KeyText)
                Num1KeyTextText = KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == Num2KeyText)
                Num2KeyTextText = KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == Num4KeyText)
                Num4KeyTextText = KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == Num5KeyText)
                Num5KeyTextText = KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == Num7KeyText)
                Num7KeyTextText = KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == Num8KeyText)
                Num8KeyTextText = KeyHelper.KeyAllKeys_code(2).ToString();


        }

        private void VKeyText_MouseDown(object sender, MouseEventArgs e)
        {
            TextBox _sender = sender as TextBox;
            _sender.Text = KeyHelper.KeyAllKeys(2);
            bool skipkey = false;
            if (KeyHelper.latestkeycount == 1 && e.Button == MouseButtons.Left)
            {
                _sender.Text = "";
                skipkey = true;
            }

            if (_sender == QKeyText)
                QKeyTextText = skipkey ? "0" : KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == WKeyText)
                WKeyTextText = skipkey ? "0" : KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == EKeyText)
                EKeyTextText = skipkey ? "0" : KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == RKeyText)
                RKeyTextText = skipkey ? "0" : KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == TKeyText)
                TKeyTextText = skipkey ? "0" : KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == YKeyText)
                YKeyTextText = skipkey ? "0" : KeyHelper.KeyAllKeys_code(2).ToString();
           
            if (_sender == Num1KeyText)
                Num1KeyTextText = skipkey ? "0" : KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == Num2KeyText)
                Num2KeyTextText = skipkey ? "0" : KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == Num4KeyText)
                Num4KeyTextText = skipkey ? "0" : KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == Num5KeyText)
                Num5KeyTextText = skipkey ? "0" : KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == Num7KeyText)
                Num7KeyTextText = skipkey ? "0" : KeyHelper.KeyAllKeys_code(2).ToString();
            if (_sender == Num8KeyText)
                Num8KeyTextText = skipkey ? "0" : KeyHelper.KeyAllKeys_code(2).ToString();

        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            UpdateConfigFile();
        }
    }


    public class KeyHelper
    {
        [DllImport("User32.dll")]
        private static extern short GetAsyncKeyState(int vKey);

        public static int latestkeycount = 0;
        public static string KeyAllKeys(int max)
        {
            latestkeycount = 0;
            string keyBuffer = string.Empty;
            bool pressed = false;
            for (int i = 0; i < 255; i++)
            {
                if (i == (int)Keys.ShiftKey)
                    continue;
                if (i == (int)Keys.Menu)
                    continue;
                if (i == (int)Keys.ControlKey)
                    continue;



                short x = GetAsyncKeyState(i);
                if ((x & 0x8000) > 0)
                {
                    if (i == (int)Keys.LShiftKey)
                    {
                        keyBuffer = "SHIFT+" + keyBuffer;
                    }
                    else if (i == (int)Keys.LMenu)
                    {
                        keyBuffer = "ALT+" + keyBuffer;
                    }
                    else if (i == (int)Keys.LControlKey)
                    {
                        keyBuffer = "CTRL+" + keyBuffer;
                    }
                    else
                    {
                        if (!pressed)
                        {
                            pressed = true;
                            keyBuffer = Enum.GetName(typeof(Keys), i) + "+" + keyBuffer;
                        }
                    }
                    latestkeycount++;
                    max--;
                    if (max == 0)
                        break;
                }

            }

            if (keyBuffer.Length > 1)
            {
                keyBuffer = keyBuffer.Remove(keyBuffer.Length - 1);
            }
            return keyBuffer;
        }


        public static string MyKeyToStr(uint val)
        {
            if (val == 0)
            {
                return string.Empty;
            }
            string keyBuffer = string.Empty;

            if ((val & 0x40000) > 0)
            {
                keyBuffer = "SHIFT+";
            }
            if ((val & 0x10000) > 0)
            {
                keyBuffer = "ALT+";
            }
            if ((val & 0x20000) > 0)
            {
                keyBuffer = "CTRL+";
            }

            int KeyVal = (int)(val & 0xFF);

            keyBuffer += Enum.GetName(typeof(Keys), KeyVal);

            return keyBuffer;
        }

        public static uint KeyAllKeys_code(int max)
        {
            uint code = 0;
            latestkeycount = 0;
            for (int i = 0; i < 255; i++)
            {
                if (i == (int)Keys.ShiftKey)
                    continue;
                if (i == (int)Keys.Menu)
                    continue;
                if (i == (int)Keys.ControlKey)
                    continue;



                short x = GetAsyncKeyState(i);
                if ((x & 0x8000) > 0)
                {
                    if (i == (int)Keys.LShiftKey)
                    {
                        code += 0x40000;
                    }
                    else if (i == (int)Keys.LMenu)
                    {
                        code += 0x10000;
                    }
                    else if (i == (int)Keys.LControlKey)
                    {
                        code += 0x20000;
                    }
                    else
                    {
                        if ((code & 0xFF) == 0)
                            code += (uint)i;
                    }
                    latestkeycount++;
                    max--;
                    if (max == 0)
                        break;
                }

            }

            return code;
        }

    }
}
