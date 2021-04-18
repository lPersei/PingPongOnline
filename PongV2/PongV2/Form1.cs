using SharpGL;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PongV2
{
    public partial class Form1 : Form
    {
        public const byte RequestConnect = 65;
        public const byte ConfirmConnect = 66;
        public const byte SideLeft = 67;
        public const byte SideRight = 68;
        public const byte StartGame = 69;
        public const byte GetPosition = 70;
        public const byte SetPosition = 71;
        public const byte SetBallPosition = 72;
        public const byte SetScore = 73;
        public const byte GameOver = 74;
        public const int MessageLength = 9;

        public delegate void DrawScene(OpenGL gl);
        DrawScene drawScene;

        IPAddress serv_ip;
        int port;

        byte scoreL = 0, scoreR = 0;

        public TcpClient tcpClient = new TcpClient();
        public NetworkStream tcpStream;

        Player player;
        Player enemy;
        Ball ball = new Ball(14.5f, 9.5f, 1, 1);

        static float MovePlayer = 8f;
        static float MoveEnemy = 8f;

        public static float MoveBallX = 14.5f;
        public static float MoveBallZ = 9.5f;

        Thread receive_thread;

        //private static Mutex mutexBall = new Mutex();

        public Form1()
        {
            InitializeComponent();
            drawScene = DrawMenu;
            Console.WriteLine("Started");
        }

        public void WaitForStart()
        {
            if (!IPAddress.TryParse(TbIP.Text, out serv_ip))
            {
                TbIP.BackColor = Color.Red;
                TbIP.Focus();
            }
            if (!Int32.TryParse(TbPort.Text, out port) || port >= 65535)
            {
                TbPort.BackColor = Color.Red;
                TbPort.Focus();
            }

            try
            {
                BtnStart.Enabled = false;
                tcpClient.Connect(serv_ip, port);
                tcpStream = tcpClient.GetStream();
                byte[] data_send = new byte[9];
                byte[] data_get = new byte[9];

                data_send[0] = RequestConnect;

                tcpStream.Write(data_send, 0, MessageLength);
                tcpStream.Read(data_get, 0, MessageLength);

                if (data_get[0] == ConfirmConnect)
                {
                    if (data_get[1] == SideLeft) //-----<
                    {
                        player = new Player(0, 8, 1, 4);
                        enemy = new Player(29, 8, 1, 4);
                    }
                    else
                    {
                        player = new Player(29, 8, 1, 4);
                        enemy = new Player(0, 8, 1, 4);
                    }

                    tcpStream.Read(data_get, 0, MessageLength);

                    if (data_get[0] == StartGame)
                    {
                        receive_thread = new Thread(new ThreadStart(Receive));
                        receive_thread.Start();

                        label1.Hide();
                        label2.Hide();
                        TbIP.Hide();
                        TbPort.Hide();
                        BtnStart.Hide();
                        BtnExit.Hide();
                        drawScene = DrawGame;

                    }
                }  
            }
            catch
            {
                MessageBox.Show("Connection Error");
                BtnStart.Enabled = true;
            }                      
        }

        void Receive()
        {         
            byte[] data_get = new byte[9];
            byte[] data_send = new byte[9];
            byte[] dataX = new byte[4];
            byte[] dataY = new byte[4];

            while (true)
            {
                try
                {
                    tcpStream.Read(data_get, 0, MessageLength);
                }
                catch
                {
                    Console.WriteLine("rec err");

                }

                Console.WriteLine("received" + data_get[0].ToString());

                if (data_get[0] == GetPosition)
                {
                    data_send[0] = SetPosition;

                    dataY = BitConverter.GetBytes(MovePlayer);
                    dataX = BitConverter.GetBytes(player.posX);

                    data_send[1] = dataX[0];
                    data_send[2] = dataX[1];
                    data_send[3] = dataX[2];
                    data_send[4] = dataX[3];

                    data_send[5] = dataY[0];
                    data_send[6] = dataY[1];
                    data_send[7] = dataY[2];
                    data_send[8] = dataY[3];

                    tcpStream.Write(data_send, 0, MessageLength);
                }

                tcpStream.Read(data_get, 0, MessageLength);
                Console.WriteLine("received" + data_get[0].ToString());

                if (data_get[0] == SetPosition)
                {
                    MoveEnemy = BitConverter.ToSingle(data_get, 5);
                }

                tcpStream.Read(data_get, 0, MessageLength);
                Console.WriteLine("received Ball" + data_get[0].ToString());

                if (data_get[0] == SetBallPosition)
                {
                    MoveBallX = BitConverter.ToSingle(data_get, 1);
                    MoveBallZ = BitConverter.ToSingle(data_get, 5);
                }
                Console.WriteLine("received X : " + MoveBallX.ToString() + "Y : " + MoveBallZ.ToString());

                tcpStream.Read(data_get, 0, MessageLength);
                
                if (data_get[0] == SetScore)
                {
                    scoreL = data_get[1];
                    scoreR = data_get[2];
                }
               
            }

        }       

        public void DrawGame(OpenGL gl)
        {
            gl.LookAt(10, 25, 15, 10, 0, 15, 0, 0, -1);

            gl.Translate(-5f, 0f, 5f);//Przesuwanie macierzy przestrzeni

            gl.DrawText(openGLControl1.Width / 2 - 120, openGLControl1.Height - 120, 1f, 1f, 1f, "Arial", 70f, scoreL.ToString() +":"+ scoreR.ToString());

            gl.Begin(OpenGL.GL_LINES);

            gl.Color(1f, 1f, 1f);

            gl.Vertex(0f, 0f, 0f);
            gl.Vertex(30f, 0f, 0f);

            gl.Vertex(30f, 0f, 0f);
            gl.Vertex(30f, 0f, 20f);

            gl.Vertex(30f, 0f, 20f);
            gl.Vertex(0f, 0f, 20f);

            gl.Vertex(0f, 0f, 20f);
            gl.Vertex(0f, 0f, 0f);


            gl.End();// Koniec rysowania kwadratów


            player.Move(MovePlayer);


            player.Draw(gl);

            enemy.Move(MoveEnemy);

            enemy.Draw(gl);         
            
            ball.Move(MoveBallX, MoveBallZ);

            ball.Draw(gl);

            if (scoreL>14 || scoreR>14)
            {
                tcpStream.Close();
                receive_thread.Abort();
                drawScene = DrawGameOver;
                BtnExit.Show();
            }
        }

        

        public void DrawMenu(OpenGL gl)
        {
            gl.DrawText(openGLControl1.Width / 2 - 150, openGLControl1.Height - 120, 1f, 1f, 1f, "Arial", 100f, "Pong");
        }

        public void DrawGameOver(OpenGL gl)
        {
            gl.DrawText(openGLControl1.Width / 2 - 150, openGLControl1.Height - 120, 1f, 1f, 1f, "Arial", 100f, "Game Over");
        }

        private void OpenGLControl1_OpenGLDraw(object sender, SharpGL.RenderEventArgs args)
        {
            OpenGL gl = this.openGLControl1.OpenGL;// Otrzymanie linku do kontrolki            

            gl.Clear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);//Czyszczenie bufera Coloru oraz bufera Głębokości

            gl.LoadIdentity(); //Resetowanie macierzy         

            drawScene(gl);

            gl.Flush(); //Zmiana buforu
        }

        private void OpenGLControl1_KeyDown(object sender, KeyEventArgs e)
        {

            if (MovePlayer - 0.5f >= 0f)
            {
                if (e.KeyData == Keys.W)
                {
                    MovePlayer -= 0.5f;
                }
            }
            if (MovePlayer + 0.5f <= 16f)
            {
                if (e.KeyData == Keys.S)
                {
                    MovePlayer += 0.5f;
                }
            }

        }

        private void BtnExit_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            tcpStream.Close();
            receive_thread.Abort();
        }

        private void BtnStart_Click(object sender, EventArgs e)
        {
            WaitForStart();
        }
    }

    public partial class Player
    {
        public float[][] PozXYZ;
        public float Width { get; set; }
        public float Height { get; set; }
        public float posX;

        public Player(float posX, float posZ, float Width, float Height)
        {
            this.PozXYZ = new float[][]
            {
                new float[]{posX,0f,posZ },
                new float[]{posX,0f,posZ+Height },
                new float[]{posX+Width,0f,posZ+Height },
                new float[]{posX+Width,0f,posZ }
            };

            this.posX = posX;
            this.Width = Width;
            this.Height = Height;
        }

        public void Draw(OpenGL gl)
        {
            gl.Begin(OpenGL.GL_QUADS);
            gl.Color(1f, 1f, 1f);
            gl.Vertex(PozXYZ[0]);
            gl.Vertex(PozXYZ[1]);
            gl.Vertex(PozXYZ[2]);
            gl.Vertex(PozXYZ[3]);
            gl.End();
        }

        public void Move(float MovePlayer)
        {
            PozXYZ[0][2] = MovePlayer;
            PozXYZ[1][2] = MovePlayer + Height;
            PozXYZ[2][2] = MovePlayer + Height;
            PozXYZ[3][2] = MovePlayer;
        }
    }

    public partial class Ball
    {
        public float[][] PozXYZ;
        public float Width { get; set; }
        public float Height { get; set; }

        public Ball(float posX, float posZ, float Width, float Height)
        {
            this.PozXYZ = new float[][]
            {
                new float[]{posX,0f,posZ },
                new float[]{posX,0f,posZ+Height },
                new float[]{posX+Width,0f,posZ+Height },
                new float[]{posX+Width,0f,posZ }
            };

            this.Width = Width;
            this.Height = Height;
        }

        public void Draw(OpenGL gl)
        {
            gl.Begin(OpenGL.GL_QUADS);
            gl.Color(1f, 1f, 1f);
            gl.Vertex(PozXYZ[0]);
            gl.Vertex(PozXYZ[1]);
            gl.Vertex(PozXYZ[2]);
            gl.Vertex(PozXYZ[3]);
            gl.End();
        }

        public void Move(float MoveBallX, float MoveBallZ)
        {
            PozXYZ[0][0] = MoveBallX;
            PozXYZ[1][0] = MoveBallX;
            PozXYZ[2][0] = MoveBallX + Width;
            PozXYZ[3][0] = MoveBallX + Width;

            PozXYZ[0][2] = MoveBallZ;
            PozXYZ[1][2] = MoveBallZ + Height;
            PozXYZ[2][2] = MoveBallZ + Height;
            PozXYZ[3][2] = MoveBallZ;
        }
    }
}
