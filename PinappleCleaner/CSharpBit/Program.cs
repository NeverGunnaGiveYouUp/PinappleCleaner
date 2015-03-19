// C# example program to demonstrate OpenTK
//
// Steps:
// 1. Create an empty C# console application project in Visual Studio
// 2. Place OpenTK.dll in the directory of the C# source file
// 3. Add System.Drawing and OpenTK as References to the project
// 4. Paste this source code into the C# source file
// 5. Run. You should see a colored triangle. Press ESC to quit.
//
// Copyright (c) 2013 Ashwin Nanjappa
// Released under the MIT License

using System;
using System.Collections.Generic;
using OpenTK;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;
using OpenTK.Input;

namespace StarterKit
{
    class DataDisplayer3D : GameWindow
    {
        Camera c;
        int stx = 019;
        int frame = 0;
        DataSet points;

        public DataDisplayer3D()
            : base(800, 600, GraphicsMode.Default, "OpenTK Quick Start Sample")
        {
            VSync = VSyncMode.On;
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            points = DataSetLoader.LoadDataSet(@"C:\Users\Admin\Documents\text18.txt");
            GL.ClearColor(0.0f, 0.0f, 0.0f,0.0f);
            GL.Enable(EnableCap.DepthTest);


            c = new Camera();
            c.Pos = new Vector3(-60, 0, 0);
            c.LookAt = new Vector3(100, 0, 0);
            c.Up = new Vector3(0, 0, 1);
        }

        protected override void OnResize(EventArgs e)
        {
            base.OnResize(e);
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

            GL.Viewport(ClientRectangle.X, ClientRectangle.Y, ClientRectangle.Width, ClientRectangle.Height);

            Matrix4 projection = Matrix4.CreatePerspectiveFieldOfView((float)Math.PI / 4, Width / (float)Height, 0.1f, 15000.0f);
            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadMatrix(ref projection);
        }

        protected override void OnUpdateFrame(FrameEventArgs e)
        {
            base.OnUpdateFrame(e);


        }

        protected override void OnRenderFrame(FrameEventArgs e)
        {
            base.OnRenderFrame(e);

            
            Matrix4 modelview = Matrix4.LookAt(c.Pos, c.LookAt, c.Up);
            GL.MatrixMode(MatrixMode.Modelview);
            GL.LoadMatrix(ref modelview);

            GL.Begin(BeginMode.Points);

           // GL.Color3(1.0f, 1.0f, 0.0f); GL.Vertex3(-1.0f, -1.0f, 4.0f);
           // GL.Color3(1.0f, 0.0f, 0.0f); GL.Vertex3(1.0f, -1.0f, 4.0f);
           // GL.Color3(0.2f, 0.9f, 1.0f); GL.Vertex3(0.0f, 1.0f, 4.0f);
            GL.Color3(1f, 0f, 0f);
            if (frame > points.Points.Length-2)
            {
                frame = 0;
            }
            foreach (Vector3 v in points.Points[frame])
            {
                GL.Vertex3(v);
            }
            GL.Color3(0f, 0f, 1f);
            foreach (Vector3 v in points.Points[frame])
            {
                GL.Vertex3(v.X  ,v.Y+0.11,v.Z);
            }
            frame += 3;
            //c.Pos.Z -= 0.1f;
            stx++;
            GL.End();

            SwapBuffers();
        }

        [STAThread]
        static void Main()
        {


            // The 'using' idiom guarantees proper resource cleanup.
            // We request 30 UpdateFrame events per second, and unlimited
            // RenderFrame events (as fast as the computer can handle).
            using (DataDisplayer3D game = new DataDisplayer3D())
            {
                game.Run(30.0);
            }
        }
    }

    public class Camera
    {
        public Vector3 Pos, LookAt, Up;
        public Camera()
        {

        }
    }

    public class DataSet
    {
        public List<Vector3>[] Points;
    }

    public static class DataSetLoader
    {
        public static DataSet LoadDataSet(string URL)
        {
            string text = System.IO.File.ReadAllText(URL);
            string[] bynewline = text.Split("\n".ToCharArray()[0]);
            string[] firstline = bynewline[0].Split(",".ToCharArray()[0]);
            int NoProperties = int.Parse(firstline[0]);
            int NoFrames = int.Parse(firstline[1]);
            int NoDimensions = int.Parse(firstline[2]);
            int NoParticles = int.Parse(firstline[3]);
            DataSet ToReturn = new DataSet();
            ToReturn.Points = new List<Vector3>[bynewline.Length - 1];
            for (int i = 1; i < bynewline.Length - 1; i++)
            {
                ToReturn.Points[i - 1] = new List<Vector3>();
                string[] bytab = bynewline[i].Split(",".ToCharArray()[0]);
                for (int p = 0; p < NoParticles; p++)
                {

                    float x = float.Parse(bytab[(p * NoDimensions) + 0]);
                    float y = float.Parse(bytab[(p * NoDimensions) + 1]);
                    float z = float.Parse(bytab[(p * NoDimensions) + 2]);
                    ToReturn.Points[i - 1].Add(new Vector3(x, y, z));
                }
            }

            return ToReturn;
        }
    }
}