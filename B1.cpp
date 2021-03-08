#include<bits/stdc++.h>
using namespace std;

int n,m,n2=0;
int a[20][20],mymap[100],name[50];

int kind[50],begin,end;

int max_weight,money0,max_day,money_gain;
int water_weight,water_price;
int food_weight,food_price;
int weather_cost[5][5];
int weather[35];

int num_vi,num_mi;

#define foodmax ((int)(max_weight/food_weight))
#define watermax ((int)(max_weight/water_weight))
#define maxnum (1000000000)

int f[32][20][402][602],ans=-maxnum,ansx,ansy,ansz,ansmoney,max_temp2[602];

ifstream file("data1.txt");
ofstream file2("test.txt");

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
	
	for(int i=1;i<=max_day;i++){
		file>>weather[i];
	}
	
	for(int day=max_day;day>=0;day--){
		for(int place=1;place<=n;place++){
			for(int i=0;i<=foodmax;i++)max_temp2[i]=-maxnum;
			int max_temp=-maxnum;
			for(int water=watermax;water>=0;water--){
				max_temp=max_temp2[foodmax];
				max_temp-=water_price*4;
				for(int food=foodmax;food>=0;food--){
					max_temp-=food_price*4;
					max_temp2[food]-=water_price*4;
					max_temp=max(max_temp,max_temp2[food]);
					max_temp=max(max_temp,-maxnum);
					if(day==max_day&&place!=end){f[day][place][water][food]=-maxnum;continue;}
					if(food*food_weight+water*water_weight>max_weight){f[day][place][water][food]=-maxnum;continue;}
					if(day==max_day){
						f[day][place][water][food]=water*water_price+food*food_price;
						continue;
					}
					
					f[day][place][water][food]=-maxnum;
					if(day==0&&money0-water*water_price-food*food_price<0)continue;
					if(day==0&&place!=begin)continue;
					
					if(place==end)f[day][place][water][food]=water*water_price+food*food_price;
					
					if((water<weather_cost[weather[day+1]][0] || food<weather_cost[weather[day+1]][1])&&kind[place]!=1)continue;
					if(kind[place]==1){
						f[day][place][water][food]=max(max_temp,f[day][place][water][food]);
						if(water<weather_cost[weather[day+1]][0] || food<weather_cost[weather[day+1]][1])continue;
					}
					
					if(kind[place]==2 && water>=3*weather_cost[weather[day+1]][0] && food>=3*weather_cost[weather[day+1]][1]){
						f[day][place][water][food]=max(f[day][place][water][food],
						f[day+1][place][water-3*weather_cost[weather[day+1]][0]][food-3*weather_cost[weather[day+1]][1]]+2*money_gain);
					}
					
					if(weather[day+1]==2){
						f[day][place][water][food]=max(f[day][place][water][food],
						f[day+1][place][water-weather_cost[2][0]][food-weather_cost[2][1]]);
					}
					else{
						for(int place2=1;place2<=n;place2++){
							if(place2==place)
								f[day][place][water][food]=max(f[day][place][water][food],
								f[day+1][place2][water-weather_cost[weather[day+1]][0]][food-weather_cost[weather[day+1]][1]]);
							else if(a[place][place2]==1 && 
							water>=2*weather_cost[weather[day+1]][0] && food>=2*weather_cost[weather[day+1]][1])
								f[day][place][water][food]=max(f[day][place][water][food],
								f[day+1][place2][water-2*weather_cost[weather[day+1]][0]][food-2*weather_cost[weather[day+1]][1]]);
						}
					}
					
					if(kind[place]==1&&f[day][place][water][food]>max_temp)max_temp=f[day][place][water][food];
					if(kind[place]==1)max_temp2[food]=max_temp;
					if(day==0&&f[day][place][water][food]>-maxnum/2){
						f[day][place][water][food]+=2*(money0-water*water_price-food*food_price);
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
	cout<<ans/2<<endl;
	ansmoney=money0-ansy*water_price-ansz*food_price;
	ans-=ansmoney*2;
	cout<<"day=0\tplace="<<name[ansx]<<"\twater="<<ansy<<"\tfood="<<ansz<<"\tmoney="<<ansmoney<<endl;
	
	for(int day=1;day<=max_day;day++){
		for(int place=1;place<=n;place++){
			if(place==ansx && ansy>=weather_cost[weather[day]][0] && ansz>=weather_cost[weather[day]][1]){
				if(f[day][place][ansy-weather_cost[weather[day]][0]][ansz-weather_cost[weather[day]][1]]==ans){
					ansx=place;
					ansy=ansy-weather_cost[weather[day]][0];
					ansz=ansz-weather_cost[weather[day]][1];
					ans=f[day][ansx][ansy][ansz];
					cout<<"day="<<day<<"\tplace="<<name[ansx]<<"\twater="<<ansy<<"\tfood="<<ansz<<"\tmoney="<<ansmoney<<endl;
					if(kind[place]!=1)goto xxx;
				}
				else if(kind[place]==2 && ansy>=3*weather_cost[weather[day]][0] && ansz>=3*weather_cost[weather[day]][1] &&
				f[day][place][ansy-3*weather_cost[weather[day]][0]][ansz-3*weather_cost[weather[day]][1]]+money_gain*2==ans){
					ansx=place;
					ansy=ansy-3*weather_cost[weather[day]][0];
					ansz=ansz-3*weather_cost[weather[day]][1];
					ans=f[day][ansx][ansy][ansz];
					ansmoney+=money_gain;
					cout<<"day="<<day<<"\tplace="<<name[ansx]<<"\twater="<<ansy<<"\tfood="<<ansz<<"\tmoney="<<ansmoney<<endl;
					if(kind[place]!=1)goto xxx;
				}
			}
			if(a[place][ansx]==1 && ansy>=2*weather_cost[weather[day]][0] && ansz>=2*weather_cost[weather[day]][1]){
				if(f[day][place][ansy-2*weather_cost[weather[day]][0]][ansz-2*weather_cost[weather[day]][1]]==ans){
					ansx=place;
					ansy=ansy-2*weather_cost[weather[day]][0];
					ansz=ansz-2*weather_cost[weather[day]][1];
					ans=f[day][ansx][ansy][ansz];
					cout<<"day="<<day<<"\tplace="<<name[ansx]<<"\twater="<<ansy<<"\tfood="<<ansz<<"\tmoney="<<ansmoney<<endl;
					if(kind[place]!=1)goto xxx;
				}
			}
			if(kind[place]==1&&ansx==place){
				for(int water=watermax;water>=ansy;water--){
					for(int food=foodmax;food>=ansz;food--){
						if(food==ansz&&water==ansy)continue;
						if(food*food_weight+water*water_weight>max_weight)continue;
						if(f[day][ansx][water][food]-(water-ansy)*water_price*4-(food-ansz)*food_price*4==ans){
							ansmoney-=(water-ansy)*water_price*2+(food-ansz)*food_price*2;
							ansx=place;
							ansy=water;
							ansz=food;
							ans=f[day][ansx][ansy][ansz];
							cout<<"day="<<day<<"\tplace="<<name[ansx]<<"\twater="<<ansy<<"\tfood="<<ansz<<"\tmoney="<<ansmoney<<endl;
							goto xxx;
						}
						
					}
				}
			}
		}
xxx:;
	}
	ansmoney+=ansy*water_price/2+ansz*food_price/2;
	cout<<ansmoney; 
} 
