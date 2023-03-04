#include<iostream>
#include<cmath> //公式里需要用到 sqrt 开根函数
#include<fstream> //该头文件用于读写文件
#include<iomanip> //该头文件用于控制数据精度
#include<sys/time.h> //标准日期时间头文件，用于计算运行时间
using namespace std;

#define timestep 20 //迭代次数
#define dT 0.005 //时间间隔
#define G 1 //引力常数
#define MAX 1024 //各信息的数组大小均设置为 1024，与粒子个数相同
#define particles 1024 //数据集中共有 1024 个粒子

#define GET_TIME(now) { \
	struct timeval t; \
	gettimeofday(&t, NULL); \
	now = t.tv_sec + t.tv_usec/1000000.0; \
}
//最后一行将 us 转换为 s，统一单位
//该结构体用于计算程序串行部分运行时间
struct information
{
	double x;
	double y;
	double z;
}pos[MAX],v[MAX];//初始化两个数组，分别为每个粒子的位置和速度
//数组内部每个元素存储了对应粒子在 x 轴， y 轴， z 轴上的对应分量
int main()
{
	int i,j,k,s;//循环变量
	double x_diff,y_diff,z_diff;//两粒子之间在三条坐标轴上的相对距离
	double dist,dist_cubed;//两粒子之间的绝对距离和绝对距离的立方
	double x_f,y_f,z_f;//两粒子之间作用力在三条坐标轴上的分量
	double m[MAX];//每个粒子的质量
	double start,end;//用于计算串行部分的开始时间和结束时间

	ifstream myfile("xxx.txt");//打开数据集文件
	for(i=0;i<=particles-1;i++){
		myfile>>m[i]>>pos[i].x>>pos[i].y>>pos[i].z>>v[i].x>>v[i].y>>v[i].z;//根据题目要求将每一行的数据写入到各数组对应的结构体成员中
	}
	GET_TIME(start);//串行开始
	for(k=1;k<=timestep;k++){//迭代次数为 timestep 次（20）
		for(i=0;i<=particles-1;i++){//遍历每个粒子
			x_f=0;
			y_f=0;
			z_f=0;
//每次遍历该粒子以外的粒子之前需要先将作用力清空，防止与上一个粒子在三个方向上所受的作用力相叠加
			for(j=0;j<=particles-1;j++){//遍历该粒子以外的所有粒子
//因为该粒子以外的所有粒子都对它有作用力
				if(i==j){//该粒子本身可以跳过
					continue;
				}
				x_diff=pos[i].x-pos[j].x;
				y_diff=pos[i].y-pos[j].y;
				z_diff=pos[i].z-pos[j].z;
//计算两粒子在三条坐标轴上的相对距离
				dist=sqrt(x_diff*x_diff+y_diff*y_diff+z_diff*z_diff);//计算两粒子之间的绝对距离
				dist_cubed=dist*dist*dist;//计算距离的立方

				x_f-=G*m[i]*m[j]/dist_cubed*x_diff;
				y_f-=G*m[i]*m[j]/dist_cubed*y_diff;
				z_f-=G*m[i]*m[j]/dist_cubed*z_diff;
//计算两粒子之间作用力在三条坐标轴上的分量
			} 
			v[i].x+=dT*x_f/m[i];
			v[i].y+=dT*y_f/m[i];
			v[i].z+=dT*z_f/m[i];
//利用 dv=f*dT/m(动量定理)来更新粒子在三条坐标轴上的速度分量
		}
		for(s=0;s<=particles-1;s++){//遍历每个粒子
			pos[s].x+=dT*v[s].x;
			pos[s].y+=dT*v[s].y;
			pos[s].z+=dT*v[s].z;
//利用 d=d0+v*dT 来更新粒子在三条坐标轴上的坐标
		}
	}
	GET_TIME(end);//获取串行部分结束时间
	cout<<end-start<<endl;//输出运行时间

	ofstream outfile;
	outfile.open("xxx_result.txt",ios::out);
//准备将结果写入 serial_result 文件中
	for(i=0;i<=particles-1;i++){
		outfile<<setprecision(15)<<m[i]<<' '<<pos[i].x<<' '<<pos[i].y<<' '<<pos[i].z<<' '<<v[i].x<<' '<<v[i].y<<' '<<v[i].z<<endl;
//将数据保留 15 位小数并按照要求把每个粒子的数据按顺序写入每行，一行数据代表一个粒子
	}
	myfile.close();
	outfile.close();
//关闭文件
	system("pause");
}