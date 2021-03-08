#include<bits/stdc++.h>
using namespace std;

int n,m,n2=0;
int a[20][20],mymap[100],name[50];

int kind[50],begin,end;

int max_weight,money0,max_day,money_gain;
int water_weight,water_price;
int food_weight,food_price;
int weather_cost[5][5];
double weather_per[5];

int num_vi,num_mi;

#define foodmax ((int)(max_weight/food_weight))
#define watermax ((int)(max_weight/water_weight))
#define maxnum (10000.0)
#define round 1000

double random(double l,double h)
{
    return l+(h-l)*(double)rand()/(double)(RAND_MAX);
}

double f[32][12][402][602],ans=-maxnum,ansmoney,max_temp2[602][3];

int ansx,ansy,ansz;

ifstream file("data4.txt");
ofstream file2("ans4.txt");

int main(){
	file>>max_weight>>money0>>max_day>>money_gain;
	file>>water_weight>>water_price;
	file>>food_weight>>food_price;
	
	file>>weather_cost[0][0]>>weather_cost[0][1];
	file>>weather_cost[1][0]>>weather_cost[1][1];
	file>>weather_cost[2][0]>>weather_cost[2][1];
	
	file>>n>>m;
	for (int i=1;i<=m;i++){
		int xx,yy;
		file>>xx>>yy;
		
		if(mymap[xx]==0){
			n2++;
			mymap[xx]=n2;
			name[n2]=xx;
			xx=n2;
		}
		else xx=mymap[xx];
		
		if(mymap[yy]==0){
			n2++;
			mymap[yy]=n2;
			name[n2]=yy;
			yy=n2;
		}
		else yy=mymap[yy];
		
		a[xx][yy]=a[yy][xx]=1;
	} 
	
	file>>begin>>end;
	begin=mymap[begin];
	end=mymap[end];
	
	file>>num_vi;
	for(int i=1;i<=num_vi;i++){
		int xx;
		file>>xx;
		kind[mymap[xx]]=1;
	}
	
	file>>num_mi;
	for(int i=1;i<=num_mi;i++){
		int xx;
		file>>xx;
		kind[mymap[xx]]=2;
	}
	
	file>>weather_per[0]>>weather_per[1]>>weather_per[2];
	
	for(int day=max_day;day>=0;day--){
		for(int place=1;place<=n;place++){
			for(int i=0;i<=foodmax;i++)max_temp2[i][0]=max_temp2[i][1]=max_temp2[i][2]=-maxnum;
			double max_temp[3]={-maxnum,-maxnum,-maxnum};
			for(int water=watermax;water>=0;water--){
				max_temp[0]=max_temp2[foodmax][0];
				max_temp[0]-=water_price*4;
				max_temp[1]=max_temp2[foodmax][1];
				max_temp[1]-=water_price*4;
				max_temp[2]=max_temp2[foodmax][2];
				max_temp[2]-=water_price*4;
				
				for(int food=foodmax;food>=0;food--){
					
					max_temp[0]-=food_price*4;
					max_temp2[food][0]-=water_price*4;
					max_temp[0]=max(max_temp[0],max_temp2[food][0]);
					max_temp[0]=max(max_temp[0],-maxnum);
					
					max_temp[1]-=food_price*4;
					max_temp2[food][1]-=water_price*4;
					max_temp[1]=max(max_temp[1],max_temp2[food][1]);
					max_temp[1]=max(max_temp[1],-maxnum);
					
					max_temp[2]-=food_price*4;
					max_temp2[food][2]-=water_price*4;
					max_temp[2]=max(max_temp[2],max_temp2[food][2]);
					max_temp[2]=max(max_temp[2],-maxnum);
					
					if(day==max_day&&place!=end){f[day][place][water][food]=-maxnum;continue;}
					if(food*food_weight+water*water_weight>max_weight){f[day][place][water][food]=-maxnum;continue;}
					if(day==max_day){
						f[day][place][water][food]=(double)(water*water_price+food*food_price);
						continue;
					}
					
					f[day][place][water][food]=-maxnum;
					if(day==0&&money0-water*water_price-food*food_price<0)continue;
					if(day==0&&place!=begin)continue;
					
					if(place==end)f[day][place][water][food]=(double)(water*water_price+food*food_price);
					
					
				{
					double f_temp[3];
					for(int today_weather=0;today_weather<=2;today_weather++){
						
						f_temp[today_weather]=f[day][place][water][food];
						
						if((water<weather_cost[today_weather][0] || food<weather_cost[today_weather][1])&&kind[place]!=1)continue;
						
						if(kind[place]==1){
							f_temp[today_weather]=max(max_temp[today_weather],f_temp[today_weather]);
							if(water<weather_cost[today_weather][0] || food<weather_cost[today_weather][1])continue;
						}
						
						if(kind[place]==2 && water>=3*weather_cost[today_weather][0] && food>=3*weather_cost[today_weather][1]){
							f_temp[today_weather]=max(f_temp[today_weather],
							f[day+1][place][water-3*weather_cost[today_weather][0]][food-3*weather_cost[today_weather][1]]+(double)(2*money_gain));
						}
						
						if(today_weather==2){
							f_temp[today_weather]=max(f_temp[today_weather],
							f[day+1][place][water-weather_cost[2][0]][food-weather_cost[2][1]]);
						}
						else{
							for(int place2=1;place2<=n;place2++){
								if(place2==place)
									f_temp[today_weather]=max(f_temp[today_weather],
									f[day+1][place2][water-weather_cost[today_weather][0]][food-weather_cost[today_weather][1]]);
								else if(a[place][place2]==1 && 
								water>=2*weather_cost[today_weather][0] && food>=2*weather_cost[today_weather][1])
									f_temp[today_weather]=max(f_temp[today_weather],
									f[day+1][place2][water-2*weather_cost[today_weather][0]][food-2*weather_cost[today_weather][1]]);
							}
						}
						if(kind[place]==1&&f_temp[today_weather]>max_temp[today_weather])max_temp[today_weather]=f_temp[today_weather];
						if(kind[place]==1)max_temp2[food][today_weather]=max_temp[today_weather];
					}
					 
					 if(f_temp[0]>=-maxnum/1.2){
					 	f[day][place][water][food]=f_temp[0]*weather_per[0]+f_temp[1]*weather_per[1]+f_temp[2]*weather_per[2];
					}
					else
						f[day][place][water][food]=-maxnum;
				}
					
					if(day==0&&f[day][place][water][food]>-maxnum/1.2){
						f[day][place][water][food]+=(double)(2*(money0-water*water_price-food*food_price));
						if(f[day][place][water][food]>ans){
							ans=f[day][place][water][food];
							ansx=place;
							ansy=water;
							ansz=food;
						}
					}
				}
			}
		}
	}
	file2<<ans/2.0<<endl;
	ansmoney=money0-ansy*water_price-ansz*food_price;
	ans-=ansmoney*2;
	file2<<"day=0\tweather=--\tplace="<<name[ansx]<<"\twater="<<ansy<<"\tfood="<<ansz<<"\tmoney="<<ansmoney<<endl<<endl;
	
	srand((unsigned)time(0));
	int suss=round;
	for(int i=1;i<=round;i++){
		int place=ansx,water=ansy,food=ansz;
		double money=ansmoney;
		
		for(int j=1;j<=max_day;j++){
			double temp=random(0.0,1.0);
			int today_weather;
			if(temp<=weather_per[0])today_weather=0;
			else if(temp<=weather_per[0]+weather_per[1])today_weather=1;
			else today_weather=2;
			
			int temp_x=-1,temp_y=water,temp_z=food;
			double temp_max=-maxnum/1.2,temp_m=money;
			
			if(kind[place]==2 && water>=3*weather_cost[today_weather][0] && food>=3*weather_cost[today_weather][1]&&
			f[j][place][water-3*weather_cost[today_weather][0]][food-3*weather_cost[today_weather][1]]+(double)(2*money_gain)>temp_max){
						temp_max=f[j][place][water-3*weather_cost[today_weather][0]][food-3*weather_cost[today_weather][1]]+(double)(2*money_gain);
						temp_x=place;
						temp_y=water-3*weather_cost[today_weather][0];
						temp_z=food-3*weather_cost[today_weather][1];
						temp_m=money+(double)(money_gain);
			}
			
			if(water>=weather_cost[today_weather][0] && food>=weather_cost[today_weather][1] &&
			f[j][place][water-weather_cost[today_weather][0]][food-weather_cost[today_weather][1]]>temp_max){
						temp_max=f[j][place][water-weather_cost[today_weather][0]][food-weather_cost[today_weather][1]];
						temp_x=place;
						temp_y=water-weather_cost[today_weather][0];
						temp_z=food-weather_cost[today_weather][1];
						temp_m=money;
			}

			if(today_weather==2)goto xxx;
			
			if(kind[place]==1){
				for(int place2=1;place2<=n;place2++){
					if(a[place2][place]==1){
						for(int k1=max(water,2*weather_cost[today_weather][0]);k1<=watermax;k1++){
							for(int k2=max(food,2*weather_cost[today_weather][1]);k2*food_weight+k1*water_weight<=max_weight;k2++){
								if(f[j][place2][k1-2*weather_cost[today_weather][0]][k2-2*weather_cost[today_weather][1]]
								-(k1-water)*4*water_price-(k2-food)*4*food_price>temp_max){
									temp_max=f[j][place2][k1-2*weather_cost[today_weather][0]][k2-2*weather_cost[today_weather][1]]
									-(k1-water)*4*water_price-(k2-food)*4*food_price;
									temp_x=place2;
									temp_y=k1-2*weather_cost[today_weather][0];
									temp_z=k2-2*weather_cost[today_weather][1];
									temp_m=money-(k1-water)*2*water_price-(k2-food)*2*food_price;
								}
							}
						}
					}
				} 
				goto xxx;
			}
			
			if(water>=2*weather_cost[today_weather][0] && food>=2*weather_cost[today_weather][1]){
						for(int place2=1;place2<=n;place2++){
							if(a[place2][place]==1 && 
							f[j][place2][water-2*weather_cost[today_weather][0]][food-2*weather_cost[today_weather][1]]>temp_max){
								temp_max=f[j][place2][water-2*weather_cost[today_weather][0]][food-2*weather_cost[today_weather][1]];
								temp_x=place2;
								temp_y=water-2*weather_cost[today_weather][0];
								temp_z=food-2*weather_cost[today_weather][1];
								temp_m=money;
							}
						}
			}
			
xxx:		if(temp_x==-1){
	
				file2<<"day="<<j<<"\tweather="<<today_weather<<"\t";
				file2<<"ÓÎÏ·Ê§°Ü"<<endl<<endl;
				suss--;
				break; 
			}
			else{
				place=temp_x;
				water=temp_y;
				food=temp_z;
				money=temp_m;
				file2<<"day="<<j<<"\tweather="<<today_weather<<"\tplace="<<name[place]<<"\twater="<<water<<"\tfood="<<food<<"\tmoney="<<money<<endl;
			}
			if(place==end){
				money+=food*food_price/2.0+water*water_price/2.0;
				file2<<"money="<<money<<endl;
				break;
			}
		}
		file2<<endl;
	}
	file2<<endl;
	file2<<"success_rate="<<(double)suss/(double)round<<endl;
} 
