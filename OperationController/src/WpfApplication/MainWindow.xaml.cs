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
using System.Windows.Controls;
using System.Windows.Media;
using System.Runtime.Remoting.Messaging;


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
        public const Int32 UM_ReceivedNOM = 0x004;
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
            if (msg == UM_ReceivedNOM)
            {
                // NOMInfo생성
                NOMInfo nomInfo = new NOMInfo();
                nomInfo = (NOMInfo)Marshal.PtrToStructure(wParam, typeof(NOMInfo));
                string MessageName = nomInfo.MsgName.ToString();


                // databuffer 입력
                byte[] msgBuffer = new byte[nomInfo.MsgLen];
                Marshal.Copy(lParam, msgBuffer, 0, nomInfo.MsgLen);

                // NOM 객체에 파싱
                NOMParser parser = new NOMParser();
                parser.nomFilePath = "GUI_NOM.xml";
                parser.parse();
                
                NMessage icdMsg = parser.getMessageObject(MessageName);
    
                NOM icdNOM = icdMsg.createNOMInstance();

                icdNOM.deserialize(msgBuffer, nomInfo.MsgLen);

                if (MessageName == "SIM_STATUS")
                {
                    // 모의기 연결 상태 표시 함수 호출
                    // ATS : 1, MSL : 2, ROS : 3, LOS : 4
                    int SIM_KIND = (int)(icdNOM.getValue("SimulatorState").toUInt());
                    
                    if (SIM_KIND == 1) enemyStatus.Fill = new SolidColorBrush(Colors.LightGreen);
                    else if (SIM_KIND == 2) missileStatus.Fill = new SolidColorBrush(Colors.LightGreen);
                    else if (SIM_KIND == 3) radarStatus.Fill = new SolidColorBrush(Colors.LightGreen);
                    else if (SIM_KIND == 4) launcherStatus.Fill = new SolidColorBrush(Colors.LightGreen);

                }
                else if (MessageName == "ROS_DETECTION")
                {
                    //탐지 여부에 따라 발사가능 여부 조절 함수 호출
                    //IsDetected : 1 = 탐지됨, 2 = 탐지 안됨
                    isDetected = (int)(icdNOM.getValue("IsDetected").toUInt());
                    if (isDetected == 1) btnLaunch.IsEnabled = true;
                    else if (isDetected == 2) btnLaunch.IsEnabled = false;
                }
                else if (MessageName == "MSL_POSITION")
                {
                    //미사일 정보 갱신
                    //IsShotDown : 1 = 격추안됨, 2 = 격추됨
                    // 격추된 경우 모의 중지 요청 송신
                    float missileNx = (icdNOM.getValue("MissileCurX").toFloat());
                    float missileNy = (icdNOM.getValue("MissileCurY").toFloat());
                    isShotDown = (int)(icdNOM.getValue("IsShotDown").toUInt());

                    if(isShotDown == 2)
                    {
                        Console.WriteLine(">> 격추에 성공하였습니다. 모의를 중지합니다.");
                        scnStop();
                    }

                }
                else if (MessageName == "ATS_POSITION")
                {
                    //공중위협 정보 갱신
                    float enemyNx = (icdNOM.getValue("EnemyCurX").toFloat());
                    float enemyNy = (icdNOM.getValue("EnemyCurY").toFloat());
                    //Console.WriteLine("현재 좌표 =(" + s_enemySx + ", " + s_enemySy + ")");
                    moveEnemy(enemyNx, enemyNy);
                    //Console.WriteLine("다음 좌표 =(" + s_enemySx + ", " + s_enemySy + ")");
                }
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
            GUIConnObj = CreateGUIConn();
            DoPlugIn(GUIConnObj);
            SetHandle(GUIConnObj, GetWindowHandle(this));
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

            NMessage icdMsg = parser.getMessageObject("SIM_CONTROL");
            NOM startNOM = icdMsg.createNOMInstance();
            startNOM.setValue("MessageId", new NUInteger(1002));
            startNOM.setValue("MessageSize", new NUInteger(12));
            startNOM.setValue("OperationType", new NUInteger(1));

            int byteSize = 0;
            byte[] nomBytes = startNOM.serialize(out byteSize);

            NOMInfo nomInfo = new NOMInfo();
            nomInfo.MsgName = "SIM_CONTROL";
            nomInfo.MsgID = 1002;
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

        // Display 관련
        static bool isSetLauncher = false;
        static bool isSetRadar = false;
        static bool isEnemyStart = false;
        static bool isEnemyTarget = false;

        private void setPosAndMessage(object sender, MouseButtonEventArgs e)
        {
            Point ClickPos = e.GetPosition((IInputElement)sender);

            // ScnDeployMsg에 저장할 실제 좌표
            float realX = (float)ClickPos.X;
            float realY = (float)ClickPos.Y;

            if (!isSetLauncher && !isSetRadar && !isEnemyStart && !isEnemyTarget)
            {
                isSetLauncher = true;

                // 각 이미지들이 Map에 찍힐 보정좌표
                float displayX = realX - 13;
                float displayY = realY - 13;

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
                img_launcher.Margin = new System.Windows.Thickness { Left = displayX, Top = displayY };
                img_launcher.Visibility = Visibility.Visible;

                setLauncherPos(realX, realY);

                Console.WriteLine("발사대 모의기 초기위치 : " + realX + " " + realY);
                Console.WriteLine(">> 발사대 모의기 초기좌표 설정");
                Console.WriteLine(">> 유도탄 모의기 초기좌표 설정");
                Console.WriteLine("===================================");

            }
            else if (isSetLauncher && !isSetRadar && !isEnemyStart && !isEnemyTarget)
            {
                isSetRadar = true;

                // 각 이미지들이 Map에 찍히고 ScnDeployMsg에 저장할 보정좌표
                float displayX = realX - 13;
                float displayY = realY - 13;
                /*
                 * TODO
                 * 
                 * 1. 레이다 이미지 표시
                 * 2. 레이다 초기 위치 설정 
                 *    (레이다 범위는 btnScnOk 을 누르면 전시)
                 * 
                 *  설정되는 위치값은 모두 ScenarioDeploy 메시지와 모의기 상태창에 띄워져야 한다.
                 */
                img_radar.Margin = new System.Windows.Thickness { Left = displayX, Top = displayY };
                img_radar.Visibility = Visibility.Visible;

                setRadarPos(realX, realY);
                Console.WriteLine("레이다 모의기 초기위치 : " + realX + " " + realY);
                Console.WriteLine(">> 레이다 모의기 초기좌표 설정");
                Console.WriteLine("===================================");
            }
            else if (isSetLauncher && isSetRadar && !isEnemyStart && !isEnemyTarget)
            {
                isEnemyStart = true;

                // 각 이미지들이 Map에 찍히고 ScnDeployMsg에 저장할 보정좌표
                float displayX = realX - 17;
                float displayY = realY - 13;
                /*
                 * TODO
                 * 
                 * 1. 공중위협 초기위치 표시
                 * 2. 공중위협 초기위치 초기좌표 설정
                 * 
                 *  설정되는 위치값은 모두 ScenarioDeploy 메시지와 모의기 상태창에 띄워져야 한다.
                 */
                img_enemyS.Margin = new System.Windows.Thickness { Left = displayX, Top = displayY };
                img_enemyS.Visibility = Visibility.Visible;
                setEnemyStartPos(realX, realY);
                Console.WriteLine("공중위협 초기위치 : " + realX + " " + realY);
                Console.WriteLine(">> 공중위협 초기위치 초기좌표 설정");
                Console.WriteLine("===================================");
            }
            else if (isSetLauncher && isSetRadar && isEnemyStart && !isEnemyTarget)
            {
                isEnemyTarget = true;

                // 각 이미지들이 Map에 찍히고 ScnDeployMsg에 저장할 보정좌표
                float displayX = realX - 17;
                float displayY = realY - 13;
                /*
                 * TODO
                 * 
                 * 1. 공중위협 타겟위치 표시
                 * 2. 공중위협 초기위치와 타겟위치를 잇는 Line 그리기
                 * 
                 *  설정되는 위치값은 모두 ScenarioDeploy 메시지와 모의기 상태창에 띄워져야 한다.
                 */
                img_enemyT.Margin = new System.Windows.Thickness { Left = displayX, Top = displayY };
                img_enemyT.Visibility = Visibility.Visible;
                setEnemyTargetPos(realX, realY);
                drawLineStartToTarget();

                Console.WriteLine("공중위협 타겟위치 : " + realX + " " + realY);
                Console.WriteLine(">> 공중위협 타겟위치 초기좌표 설정");
                Console.WriteLine(">> 공중위협 초기 & 타겟위치 간 Line 생성");
                Console.WriteLine("===================================");
            }
        }

        // 초기 좌표 string 변수
        static string initText = "초기 좌표";

        // ScenarioDeploy Message 에 담을 필드
        // 발사대 필드
        static float s_launcherX = default;
        static float s_launcherY = default;
        static uint s_missileStock = 4;

        // 미사일 필드
        static float s_missileX = default;
        static float s_missileY = default;
        static float s_missileSpeed = 5;

        // 레이다 필드
        static float s_radarX = default;
        static float s_radarY = default;
        public const int s_radarRange = 200;    

        // 공중위협 필드
        static float s_enemySx = default;
        static float s_enemySy = default;
        static float s_enemyTx = default;
        static float s_enemyTy = default;
        static uint s_enemyType = default;
        static int s_enemySpeed = default;

        private void btnScnDeployClick(object sender, RoutedEventArgs e)
        {
            Console.WriteLine("===================================");
            Console.WriteLine(">> btnScnDeployClick() : ");

            NOMParser parser = new NOMParser();
            parser.nomFilePath = "GUI_NOM.xml";
            parser.parse();

            NMessage icdMsg = parser.getMessageObject("SCN_DEPLOY");
            NOM startNOM = icdMsg.createNOMInstance();
            startNOM.setValue("MessageId", new NUInteger(1001));
            startNOM.setValue("MessageSize", new NUInteger(68));

            startNOM.setValue("EnemyInitX", new NFloat(s_enemySx));
            startNOM.setValue("EnemyInitY", new NFloat(s_enemySy));
            startNOM.setValue("EnemyDestX", new NFloat(s_enemyTx));
            startNOM.setValue("EnemyDestY", new NFloat(s_enemyTy));
            startNOM.setValue("EnemySpeed", new NFloat(s_enemySpeed));
            startNOM.setValue("EnemyType", new NUInteger(s_enemyType));
            startNOM.setValue("EnemyAngle", new NFloat(0));
            startNOM.setValue("MissileInitX", new NFloat(s_missileX));
            startNOM.setValue("MissileInitY", new NFloat(s_missileY));
            startNOM.setValue("MissileSpeed", new NFloat(s_missileSpeed));
            startNOM.setValue("RadarInitX", new NFloat(s_radarX));
            startNOM.setValue("RadarInitY", new NFloat(s_radarY));
            startNOM.setValue("LauncherInitX", new NFloat(s_launcherX));
            startNOM.setValue("LauncherInitY", new NFloat(s_launcherY));
            startNOM.setValue("LauncherStock", new NUInteger(s_missileStock));

            int byteSize = 0;
            byte[] nomBytes = startNOM.serialize(out byteSize);

            NOMInfo nomInfo = new NOMInfo();
            nomInfo.MsgName = "SCN_DEPLOY";
            nomInfo.MsgID = 1001;
            nomInfo.MsgLen = byteSize;

            IntPtr ptr = Marshal.AllocHGlobal(nomInfo.MsgLen);

            Marshal.Copy(nomBytes, 0, ptr, nomInfo.MsgLen);
            SendMsg(GUIConnObj, nomInfo, ptr);
        }

        private void btnScnStopClick(object sender, RoutedEventArgs e) 
        {
            Console.WriteLine("===================================");
            Console.WriteLine(">> btnScnStopClick() : ");

            NOMParser parser = new NOMParser();
            parser.nomFilePath = "GUI_NOM.xml";
            parser.parse();

            NMessage icdMsg = parser.getMessageObject("SIM_CONTROL");
            NOM startNOM = icdMsg.createNOMInstance();
            startNOM.setValue("MessageId", new NUInteger(1002));
            startNOM.setValue("MessageSize", new NUInteger(12));
            startNOM.setValue("OperationType", new NUInteger(3));

            int byteSize = 0;
            byte[] nomBytes = startNOM.serialize(out byteSize);

            NOMInfo nomInfo = new NOMInfo();
            nomInfo.MsgName = "SIM_CONTROL";
            nomInfo.MsgID = 1001;
            nomInfo.MsgLen = byteSize;

            IntPtr ptr = Marshal.AllocHGlobal(nomInfo.MsgLen);

            Marshal.Copy(nomBytes, 0, ptr, nomInfo.MsgLen);
            SendMsg(GUIConnObj, nomInfo, ptr);
        }

        private void scnStop()
        {
            NOMParser parser = new NOMParser();
            parser.nomFilePath = "GUI_NOM.xml";
            parser.parse();

            NMessage icdMsg = parser.getMessageObject("SCN_CONTROL");
            NOM startNOM = icdMsg.createNOMInstance();
            startNOM.setValue("MessageId", new NUInteger(1002));
            startNOM.setValue("MessageSize", new NUInteger(12));
            startNOM.setValue("OperationType", new NUInteger(3));

            int byteSize = 0;
            byte[] nomBytes = startNOM.serialize(out byteSize);

            NOMInfo nomInfo = new NOMInfo();
            nomInfo.MsgName = "SCN_CONTROL";
            nomInfo.MsgID = 1001;
            nomInfo.MsgLen = byteSize;

            IntPtr ptr = Marshal.AllocHGlobal(nomInfo.MsgLen);

            Marshal.Copy(nomBytes, 0, ptr, nomInfo.MsgLen);
            SendMsg(GUIConnObj, nomInfo, ptr);
        }

        private void setLauncherPos(float x, float y)
        {
            launcherPos.Text = x + ", " + y;
            
            s_launcherX = x;
            s_launcherY = y;

            setMissilePos(x, y);
        }
        private void setMissilePos(float x, float y)
        {
            missilePos.Text = x + ", " + y;
            s_missileX = x;
            s_missileY = y;
        }
        private void setRadarPos(float x, float y)
        {
            radarPos.Text = x + ", " + y;

            s_radarX = x;
            s_radarY = y;
        }
        private void setEnemyStartPos(float x, float y)
        {
            enemyPos.Text = x + ", " + y;
            s_enemySx = x;
            s_enemySy = y;
        }
        private void setEnemyTargetPos(float x, float y)
        {
            s_enemyTx = x;
            s_enemyTy = y;
        }

        private void drawLineStartToTarget() 
        {
            Line line = createLine(s_enemySx, s_enemySy, s_enemyTx, s_enemyTy, Brushes.Red, 2, null);

            mapGrid.Children.Add(line);
        }
        private Line createLine(float x1, float y1, float x2, float y2, Brush brush, double thickness, DoubleCollection dashStyle)
        {
            Line line = new Line();

            line.X1 = x1;
            line.Y1 = y1;
            line.X2 = x2;
            line.Y2 = y2;

            line.Stroke = brush;
            line.StrokeThickness = thickness;
            line.StrokeDashArray = dashStyle;

            return line;
        }
        // 공중위협 종류 comboBox
        private void enemyItemsComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ComboBox comboBox = enemyItemsComboBox;
            string text = (comboBox.SelectedItem as ComboBoxItem).Content.ToString();

            if (text == "항공기") s_enemyType = 1;
            else s_enemyType = 2;

            Console.WriteLine(">> 공중위협 선택 : " + text);
            Console.WriteLine("===================================");
        }
        // 공중위협 속도 comboBox
        private void enemySpeedComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ComboBox comboBox = enemySpeedComboBox;
            string text = (comboBox.SelectedItem as ComboBoxItem).Content.ToString();
            int speed = int.Parse(text);
            s_enemySpeed = speed;

            Console.WriteLine(">> 공중위협 속도 : " + text);
            Console.WriteLine("===================================");
        }

        private void btnScnOkClick(object sender, RoutedEventArgs e)
        {
            Console.WriteLine(">> btnScnOkClick() : ");
            Console.WriteLine("===================================");
            /*
             * 아래 모든 필드가 채워져야만 ScenarioDeploy Message 에 담겨야함
             * 
             * 공중위협 초기좌표(x,y)
             * 공중위협 목적좌표(x,y)
             * 공중위협 속력, 유형
             * 유도탄 초기좌표(x, y)
             * 레이다 초기좌표(x, y)
             * 발사대 초기좌표(x, y)
             * 
             */
            if (isAllCheckedStatus())
            {
                // 모두 값이 채워져 있으면 SCN_DEPLOY에 setValue.
                // 여기서 SCN_DEPLOY 에 setValue
                Console.WriteLine(">> 시나리오 배포 준비가 되었습니다. ");
                Console.WriteLine("===================================");
                img_radar_range.Visibility = Visibility.Visible;
                img_radar_range.Margin = new System.Windows.Thickness { Left = s_radarX - 200, Top = s_radarY - 200 };
                // 임의로 true로 한거임 지우셈 나중에
                //btnLaunch.IsEnabled = true;
                // 임의로 true로 한거임 지우셈 나중에
            }
            else
            {
                Console.WriteLine(">> 시나리오 배포가 불가능 합니다. 필드를 채워주세요. ");
            }
        }

        private void btnScnClearClick(object sender, RoutedEventArgs e)
        {
            Console.WriteLine(">> btnScnClearClick() : ");
            Console.WriteLine("===================================");
            // 발사대 필드
            s_launcherX = default;
            s_launcherY = default;
            // 미사일 필드
            s_missileX = default;
            s_missileY = default;
            s_missileStock = 4;
            // 레이다 필드
            s_radarX = default;
            s_radarY = default;
            // 공중위협 필드
            s_enemySx = default;
            s_enemySy = default;
            s_enemyTx = default;
            s_enemyTy = default;
            s_enemyType = default;
            s_enemySpeed = default;
            // Display 관련
            isSetLauncher = false;
            isSetRadar = false;
            isEnemyStart = false;
            isEnemyTarget = false;
            // 상태창 좌표 초기화
            launcherPos.Text = initText;
            missilePos.Text = initText;
            radarPos.Text = initText;
            enemyPos.Text = initText;

            // 공중위협 초기 - 타깃 사이 선 삭제, 각종 이미지 다시 추가
            allImgSetVisibilityHidden();
            mapGrid.Children.Clear();
            allImgSetToMapGrid();
            // 콤보박스 초기화


        }
        private void allImgSetVisibilityHidden()
        {
            img_launcher.Visibility = Visibility.Hidden;
            img_radar.Visibility = Visibility.Hidden;
            img_enemyS.Visibility = Visibility.Hidden;
            img_enemyT.Visibility = Visibility.Hidden;
            img_enemyE.Visibility = Visibility.Hidden;
            img_missile.Visibility = Visibility.Hidden;
            img_radar_range.Visibility = Visibility.Hidden;
        }

        private void allImgSetToMapGrid()
        {
            mapGrid.Children.Add(img_map);
            mapGrid.Children.Add(img_launcher);
            mapGrid.Children.Add(img_radar);
            mapGrid.Children.Add(img_enemyS);
            mapGrid.Children.Add(img_enemyT);
            mapGrid.Children.Add(img_enemyE);
            mapGrid.Children.Add(img_missile);
            mapGrid.Children.Add(img_radar_range);
        }

        private bool isAllCheckedStatus()
        {
            if (s_enemySx != default && s_enemySy != default &&
                s_enemyTx != default&& s_enemyTy != default &&
                s_enemySpeed != default &&
                s_enemyType != default &&
                s_missileX != default && s_missileY != default &&
                s_radarX != default && s_radarY != default &&
                s_launcherX != default && s_launcherY != default) return true;

            else return false;
        }

        // 유도탄, 공중위협 로직
        static int isDetected;         // 탐지 여부
        static int isShotDown;         // 격추 여부
        static int MISSILE_ENEMY_MARGIN = 25;

        /* ROS_DETECTION 오면 enemyDetected 를 true로 변경 */
        /* ROS_DETECTION 오면 btnLaunch 를 true로 변경 */

        private void btnLaunchClick(object sender, RoutedEventArgs e)
        {
            Console.WriteLine(">> btnLaunchClick() : ");
            Console.WriteLine("===================================");

            NOMParser parser = new NOMParser();
            parser.nomFilePath = "GUI_NOM.xml";
            parser.parse();

            NMessage icdMsg = parser.getMessageObject("MSL_LAUNCH");
            NOM startNOM = icdMsg.createNOMInstance();
            startNOM.setValue("MessageId", new NUInteger(1005));
            startNOM.setValue("MessageSize", new NUInteger(8));

            int byteSize = 0;
            byte[] nomBytes = startNOM.serialize(out byteSize);

            NOMInfo nomInfo = new NOMInfo();
            nomInfo.MsgName = "MSL_LAUNCH";
            nomInfo.MsgID = 1005;
            nomInfo.MsgLen = byteSize;

            IntPtr ptr = Marshal.AllocHGlobal(nomInfo.MsgLen);

            Marshal.Copy(nomBytes, 0, ptr, nomInfo.MsgLen);
            SendMsg(GUIConnObj, nomInfo, ptr);

            // 미사일 위치 조정 및 display
            img_missile.Margin = new System.Windows.Thickness { Left = s_missileX - MISSILE_ENEMY_MARGIN, Top = s_missileY - MISSILE_ENEMY_MARGIN };
            img_missile.Visibility = Visibility.Visible;
            Console.WriteLine(">> s_missileX, s_missileY : " + s_missileX + ", " + s_missileY);
            Console.WriteLine("===================================");
        }

        private void btnScnStartClick(object sender, RoutedEventArgs e)
        {
            Console.WriteLine(">> btnScnStartClick() : ");
            Console.WriteLine(">> 모의를 시작합니다.  ");
            Console.WriteLine("===================================");

            // 공중위협 위치 조정 및 display
            img_enemyE.Margin = new System.Windows.Thickness { Left = s_enemySx - MISSILE_ENEMY_MARGIN, Top = s_enemySy - MISSILE_ENEMY_MARGIN };
            img_enemyE.Visibility = Visibility.Visible;

            NOMParser parser = new NOMParser();
            parser.nomFilePath = "GUI_NOM.xml";
            parser.parse();

            NMessage icdMsg = parser.getMessageObject("SIM_CONTROL");
            NOM startNOM = icdMsg.createNOMInstance();
            startNOM.setValue("MessageId", new NUInteger(1002));
            startNOM.setValue("MessageSize", new NUInteger(12));
            startNOM.setValue("OperationType", new NUInteger(1));

            int byteSize = 0;
            byte[] nomBytes = startNOM.serialize(out byteSize);

            NOMInfo nomInfo = new NOMInfo();
            nomInfo.MsgName = "SIM_CONTROL";
            nomInfo.MsgID = 1002;
            nomInfo.MsgLen = byteSize;

            IntPtr ptr = Marshal.AllocHGlobal(nomInfo.MsgLen);

            Marshal.Copy(nomBytes, 0, ptr, nomInfo.MsgLen);
            SendMsg(GUIConnObj, nomInfo, ptr);

            Console.WriteLine(">> s_enemySx, s_enemySy : " + s_enemySx + ", " + s_enemySy);
            Console.WriteLine("===================================");
        }
        
        /* MSL_POSITION 에 따라 점 찍고 이미지 이동*/
        private void moveMissile(float x, float y, int isShotDown)
        {
            // 격추 안됐다면
            if (isShotDown == 1)
            {
                float nx = x;
                float ny = y;
                // 현재좌표(s_missileX, s_missileY) 점 찍기
                drawPoint(s_missileX, s_missileY, nx, ny);
                // 새로 넘어온 좌표로 이미지 이동시키기
                img_missile.Margin = new System.Windows.Thickness { Left = nx - MISSILE_ENEMY_MARGIN, Top = ny - MISSILE_ENEMY_MARGIN };
                // 다음 좌표로 갱신 
                s_missileX = nx;
                s_missileY = ny;
            }
            // 격추 됐다면
            else if (isShotDown == 2)
            {
                // 이벤트로그에 모의 중지됨을 전시
                Console.WriteLine(">> 격추에 성공했습니다. 모의를 중지합니다  ");

            }
        }
        /* moveMissile 테스트 코드 */
        private void btnMoveMissileTestClick(object sender, RoutedEventArgs e)
        {
            Console.WriteLine(">> 미사일 이동 테스트 : ");
            Console.WriteLine("===================================");

            int cnt = 0;
            float nx = s_missileX;
            float ny = s_missileY;

            while (cnt < 1000)
            {
                nx += 1;
                ny += 1;

                moveMissile(nx, ny, 1);

                cnt++;
                Console.WriteLine("[" + cnt + "]: (" + nx + ", " + ny + ")");
            }
        }

        /* ATS_POSITION 에 따라 이미지 이동 */
        private void moveEnemy(float x, float y)
        {
            // 새로 넘어온 좌표로 이미지 이동시키기
            float nx = x;
            float ny = y;
            
            // 새로 넘어온 좌표로 이미지 이동시키기
            img_enemyE.Margin = new System.Windows.Thickness { Left = nx - MISSILE_ENEMY_MARGIN, Top = ny - MISSILE_ENEMY_MARGIN };
            // 다음 좌표로 갱신 
            s_enemySx = nx;
            s_enemySy = ny;
        }
        /* moveMissile 테스트 코드 */
        private void btnMoveEnemyTestClick(object sender, RoutedEventArgs e)
        {
            Console.WriteLine(">> 공중위협 이동 테스트 : ");
            Console.WriteLine("===================================");

            int cnt = 0;
            float nx = s_enemySx;
            float ny = s_enemySy;

            while (cnt < 500)
            {
                nx += 1;
                ny += 1;

                moveEnemy(nx, ny);

                cnt++;
                Console.WriteLine("[" + cnt + "]: (" + nx + ", " + ny + ")");
            }
        }

        /* 점 찍기 */
        private void drawPoint(float cx, float cy, float nx, float ny)
        {
            Line missilePoint = createLine(cx, cy, nx, ny, Brushes.Blue, 2, null);
            mapGrid.Children.Add(missilePoint);
        }


    }
}
