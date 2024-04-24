using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Threading;
using WpfApplication;
using nframework.nom;
using System.Windows.Input;
using System.Threading.Tasks;
using System.Windows.Shapes;


namespace WpfApplication1
{   /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        static class NativeMethods
        {
            [DllImport("kernel32.dll", SetLastError = true)]
            [return: MarshalAs(UnmanagedType.Bool)]
            public static extern bool AllocConsole();
        }

        [DllImport("GUIConnectord.dll", CallingConvention = CallingConvention.Cdecl)]
        extern public static IntPtr CreateGUIConn();
        [DllImport("GUIConnectord.dll", CallingConvention = CallingConvention.Cdecl)]
        extern public static void DeleteGUIConn(IntPtr GUIConn);
        [DllImport("GUIConnectord.dll", CallingConvention = CallingConvention.Cdecl)]
        extern public static void SetHandle(IntPtr GUIConn, IntPtr hwnd);
        [DllImport("GUIConnectord.dll", CallingConvention = CallingConvention.Cdecl)]
        extern public static void SendMsg(IntPtr GUIConn, NOMInfo nomInfo, IntPtr nomBytes);
        [DllImport("GUIConnectord.dll", CallingConvention = CallingConvention.Cdecl)]
        extern public static void DoPlugIn(IntPtr GUIConn);
        [DllImport("GUIConnectord.dll", CallingConvention = CallingConvention.Cdecl)]
        extern public static void DoPlugOut(IntPtr GUIConn);

        public IntPtr GUIConnObj;

        //Window Message 사용자 정의
        public const Int32 WM_USER = 0x0400;
        public const Int32 WM_SEND_DATA = WM_USER + 0x01;
        public const Int32 UM_ReflectedNOM = WM_USER + 0x02;
        public static int recvCnt = 0;

        public MainWindow()
        {
            InitializeComponent();
            NativeMethods.AllocConsole();
        }

        //윈도우 핸들 얻기
        public IntPtr GetWindowHandle(Window window)
        {
            return new WindowInteropHelper(window).Handle;
        }

        //WndProc 함수 등록하기
        protected override void OnSourceInitialized(EventArgs e)
        {
            base.OnSourceInitialized(e);
            HwndSource source = PresentationSource.FromVisual(this) as HwndSource;
            source.AddHook(WndProc);
        }

        //WndProc 함수 : C++로부터 수신한 NOM data 처리
        private IntPtr WndProc(IntPtr hwnd, Int32 msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            if (msg == WM_SEND_DATA)
            {
                // if need be, write your code
            }
            else if (msg == UM_ReflectedNOM)
            {
                NOMInfo nomInfo = new NOMInfo();
                byte[] msgBuffer = new byte[nomInfo.MsgLen];
                Marshal.Copy(lParam, msgBuffer, 0, nomInfo.MsgLen);

                nomInfo = (NOMInfo)Marshal.PtrToStructure(wParam, typeof(NOMInfo));
                //listBox.Items.Add(nomInfo.MsgName.ToString());
            }
            else
            {
                handled = false;
            }
            return IntPtr.Zero;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // if need be, write your code
        }

        /// <summary>
        /// C++ DLL에서 송신 쓰레드를 생성하여 C#으로 SendMessage 주기적으로 송신(10ms)
        /// 시작과 함께 HWND를 C++ DLL로 전달
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>

        /// <summary>
        /// C++ DLL의 송신 쓰레드 종료 시킴
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            
        }
        /// <summary>
        /// C#에서 C++로 메시지 송신
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnCreateObj_Click(object sender, RoutedEventArgs e)
        {
            GUIConnObj = CreateGUIConn();
        }
        private void btnSetHadle_Click(object sender, RoutedEventArgs e)
        {

            SetHandle(GUIConnObj, GetWindowHandle(this));
        }
        private void btnSendMsg_Click(object sender, RoutedEventArgs e)
        {
            NOMParser parser = new NOMParser();
            parser.nomFilePath = "GUI_NOM.xml";
            parser.parse();

            NMessage icdMsg = parser.getMessageObject("MSL_LAUNCH");
            NOM startNOM = icdMsg.createNOMInstance();

            int byteSize = 0;
            byte[] nomBytes = startNOM.serialize(out byteSize);

            NOMInfo nomInfo = new NOMInfo();
            nomInfo.MsgName = "MSL_LAUNCH";
            nomInfo.MsgID = 1005;
            nomInfo.MsgLen = byteSize;

            IntPtr ptr = Marshal.AllocHGlobal(nomInfo.MsgLen);

            Marshal.Copy(nomBytes, 0, ptr, nomInfo.MsgLen);
            SendMsg(GUIConnObj, nomInfo, ptr);
        }
        private void btnDeleteObj_Click(object sender, RoutedEventArgs e)
        {
            DeleteGUIConn(GUIConnObj);
            GUIConnObj = IntPtr.Zero;
        }

        private void btnPlugIn_Click(object sender, RoutedEventArgs e)
        {
            DoPlugIn(GUIConnObj);
        }

        private void btnPlugOut_Click(object sender, RoutedEventArgs e)
        {
            DoPlugOut(GUIConnObj);
        }

        private void enemySpeedComboBox_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
        {

        }

        static bool isSetLauncher = false;
        static bool isSetRadar = false;
        static bool isEnemyStart = false;
        static bool isEnemyTarget = false;

        private void Img_ClickTest_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Point ClickPos = e.GetPosition((IInputElement)sender);

            float ClickX = (float)ClickPos.X;
            float ClickY = (float)ClickPos.Y;

            
            if (!isSetLauncher && !isSetRadar && !isEnemyStart && !isEnemyTarget)
            {
                isSetLauncher = true;
                /*
                 * TODO
                 * 
                 * 1. 발사대 이미지 표시
                 * 2. 발사대 초기 위치 설정
                 * 3. 유도탄 초기 위치 설정
                 * 
                 * 설정되는 위치값은 모두 ScenarioDeploy 메시지와 모의기 상태창에 띄워져야 한다.
                 * 
                 */
                Console.WriteLine("발사대 모의기 초기위치 : " + ClickX + " " + ClickY);
                Console.WriteLine(">> 발사대 모의기 초기좌표 설정");
                Console.WriteLine(">> 유도탄 모의기 초기좌표 설정");
                Console.WriteLine("===================================");

            }
            else if (isSetLauncher && !isSetRadar && !isEnemyStart && !isEnemyTarget)
            {
                isSetRadar = true;
                /*
                 * TODO
                 * 
                 * 1. 레이다 이미지 표시
                 * 2. 레이다 초기 위치 설정 
                 *    (레이다 범위는 btnScnOk 을 누르면 전시)
                 * 
                 *  설정되는 위치값은 모두 ScenarioDeploy 메시지와 모의기 상태창에 띄워져야 한다.
                 */
                Console.WriteLine("레이다 모의기 초기위치 : " + ClickX + " " + ClickY);
                Console.WriteLine(">> 레이다 모의기 초기좌표 설정");
                Console.WriteLine("===================================");
            }
            else if (isSetLauncher && isSetRadar && !isEnemyStart && !isEnemyTarget)
            {
                isEnemyStart = true;
                /*
                 * TODO
                 * 
                 * 1. 공중위협 초기위치 표시
                 * 2. 공중위협 초기위치 초기좌표 설정
                 * 
                 *  설정되는 위치값은 모두 ScenarioDeploy 메시지와 모의기 상태창에 띄워져야 한다.
                 */
                Console.WriteLine("공중위협 초기위치 : " + ClickX + " " + ClickY);
                Console.WriteLine(">> 공중위협 초기위치 초기좌표 설정");
                Console.WriteLine("===================================");
            }
            else if (isSetLauncher && isSetRadar && isEnemyStart && !isEnemyTarget)
            {
                isEnemyTarget = true;
                /*
                 * TODO
                 * 
                 * 1. 공중위협 타겟위치 표시
                 * 2. 공중위협 초기위치와 타겟위치를 잇는 Line 그리기
                 * 
                 *  설정되는 위치값은 모두 ScenarioDeploy 메시지와 모의기 상태창에 띄워져야 한다.
                 */
                Console.WriteLine("공중위협 타겟위치 : " + ClickX + " " + ClickY);
                Console.WriteLine(">> 공중위협 타겟위치 초기좌표 설정");
                Console.WriteLine(">> 공중위협 초기 & 타겟위치 간 Line 생성");
                Console.WriteLine("===================================");
            }

            //launcherImage.Margin = new System.Windows.Thickness { Left = ClickX, Top = ClickY };
        }

    }
}
