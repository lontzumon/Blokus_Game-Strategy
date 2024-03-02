#include<stdio.h>
#include<string.h>
#include<stdlib.h>



void writeFile(char *filename, char *content)
{
    /*
    * Input: absolute file path (file name if it is in the same directory with the file),
    *        content of the file
    * Write the content into a specified file
    *
    *
    */

    FILE *fp;
    fp = fopen(filename,"w");
    fprintf(fp,"%s",content);
    fclose(fp);

}
char* readFile(char *filename){
    /*
     Input: absolute file path (file name if it is in the same directory with the file)
     Ouput: contents of the file
     Read the whole content of a specified file with its name and return the content
     as a string pointer
    
    
    */
    
    FILE *fp;
    char temp[1000] = {'\0'};
    char *return_string = "";
    fp = fopen(filename, "r");
    
    if (fp == NULL){
        printf("Can't open file\n");

    }

    char *temp3 = "";
    
    while(fgets(temp, 1000, fp)!=NULL)
    {
        return_string = malloc((strlen(temp)+strlen(return_string)+1)*sizeof(char));/*new char[strlen(temp)+strlen(return_string)+1]*/
        strcpy(return_string,temp3);
        temp3 = malloc((strlen(temp)+strlen(temp3)+1)*sizeof(char));
        strcat(return_string,temp);
        strcpy(temp3,return_string);
    }
    fclose(fp);
    return return_string;
}

int near_block(int x,int y,char *coordinate,char enemy_color){
	int near=0;
	if(x+1<14 && coordinate[x*28+2*y+28]==enemy_color){
		near++;    
	}
	if(x-1>-1 && coordinate[x*28+2*y-28]==enemy_color){
		near++; 
	}
	if(y+1<14 && coordinate[x*28+2*y+2]==enemy_color){
		near++;
	}
	if(y-1>-1 && coordinate[x*28+2*y-2]==enemy_color){
		near++;  
	}
	return near; 
}


int first_put_test(int x,int y,char *coordinate,char color){
	int near=0;
	int edge=0;
	if(x+1<14 && coordinate[x*28+2*y+28]==color){
		near++;  
	}
	if(x-1>-1 && coordinate[x*28+2*y-28]==color){
		near++;  
	}
	if(y+1<14 && coordinate[x*28+2*y+2]==color){
		near++;  
	}
	if(y-1>-1 && coordinate[x*28+2*y-2]==color){
		near++;  
	}
	if(near==0){
		/*printf("i:%d j:%d\n",x,y);*/
		if(x+1<14&&y+1<14&&coordinate[x*28+2*y+28+2]==color){
			edge++;
		}
		if(x-1>-1&&y+1<14&&coordinate[x*28+2*y-28+2]==color){
			edge++;
		}
		if(x+1<14&&y-1>-1&&coordinate[x*28+2*y+28-2]==color){
			edge++;
		}
		if(x-1>-1&&y-1>-1&&coordinate[x*28+2*y-28-2]==color){
			edge++;
		}
		if(edge!=0){
			return 1;  /* 1 represents true*/
		}
		else{
			return 2;  /* 2 represents false*/
		}
	}
	else{
		return 2;   /*  2 represents false*/
	}
}



int count_edge_number(char *map,char color){  //count the number of edge if I put the block already
	int i;
	int j;
	int put_test1_1;
	int edge_number=0;
	for(i=0;i<14;i++){    /*i is row  (map)*/
		for(j=0;j<14;j++){  /*j is column  (map)*/
			if(map[i*28+2*j]=='N'){
				if(first_put_test(i,j,map,color)==1){
					edge_number+=1;	
				}
				
			}
		}
	}
	return edge_number;
}


int second_put_test(int x,int y,char *coordinate,char color){ // if the block near to the other my_block,then return 2(false)
	/*printf("second_put_test,coordinate:\n%s \n",coordinate);
	printf("second_put_test,coordinate-1:\n%s \n",coordinate-1);*/
	if(x+1<14 && coordinate[x*28+2*y+28]==color){
		return 2;    /*  2 represents false*/
	}
	if(x-1>-1 && coordinate[x*28+2*y-28]==color){
		return 2;  
	}
	if(y+1<14 && coordinate[x*28+2*y+2]==color){
		return 2;  
	}
	if(y-1>-1 && coordinate[x*28+2*y-2]==color){
		return 2;  
	}
	return 1;  /* 1 represents true*/
}







void can_put_all(int x_difference,int y_difference,int *block,char *map,int size,char color){
	int i;
	int x_coordinate,y_coordinate;
	/*printf("%s\n","can put all");*/
	for(i=0;i<size;i++){
		x_coordinate=*(block+2*i)+x_difference;  /*  x_coordinate is from 0 to 13*/
		y_coordinate=*(block+2*i+1)+y_difference;  /*  y_coordinate is from 0 to 13*/
		map[x_coordinate*28+2*y_coordinate]=color;
	}
}


void edit_left(char *left_string,int string_length,int block_number){
	char put_string[1000]="\0";
	int i;
	strcat(put_string,"\n[");
	/*printf("block_number:%d\n",block_number);
	printf("string_length:%d\n",string_length);*/
	for(i=1;i<22;i++){
		if(i==block_number){
			strcat(put_string,"0");
		}
		else{
			if(left_string[string_length-1-1-2*(21-i)]=='1'){
				strcat(put_string,"1");
			}
			else{
				strcat(put_string,"0");
			}
		}
		if(i==21){
			strcat(put_string,"]");
		}
		else{
			strcat(put_string,",");
		}
		
	}
	strcat(left_string,put_string);
}

void counterclock90(int *x,int*y){
	int change;
	change=*x;
	*x= -(*y);
	*y= change;
}
void reverse(int *x){
	*x = -*x;
}
int main(int argc ,const char *argv[]){  /*   0    argv[1] "Blue"or"Red"   2 board_File  3 left_File  4 step_File*/ 
	int i,j,k,l,m,n,o;
	int can_put_test;
	int left_length;
	int x_difference,y_difference;
	int x_coordinate,y_coordinate;
	int block_area;
	int end=0;
	int block_enemy_test=0;
	int try_near=0;
	int future_way=0;
	int edge_coordinate[100];
	int edge_number=0;
	int enemy_edge_coordinate[100];
	int enemy_edge_number=0;
	int truely_put[9];/* 1:x 2:y 3:block 4:block_area 5:turn rotate 6:turn reverse 7:block_enemy 8:the way can put in future 9:near the enemy*/
	truely_put[6]=-1;
	truely_put[7]=-1;
	truely_put[8]=-1;
	int rotate_test=0;
	int reverse_test=0;
	char enemy_color;
	/*int block_number=-1;*/
	char color;   /*                         it will change red or blue*/
	int one[1][2]={{0,0}};
	int two[2][2]={{0,0},{0,1}};
	int three[3][2]={{0,0},{0,1},{1,1}};
	int four[3][2]={{0,0},{0,1},{0,2}};
	int five[4][2]={{0,0},{0,1},{1,0},{1,1}};
	int six[4][2]={{0,0},{0,1},{0,2},{1,0}};
	int seven[4][2]={{0,0},{0,1},{0,2},{1,1}};
	int eight[4][2]={{0,0},{0,1},{1,1},{1,2}};
	int nine[4][2]={{0,0},{0,1},{0,2},{0,3}};
	int ten[5][2]={{0,0},{0,1},{0,2},{1,0},{1,1}};
	int eleven[5][2]={{0,0},{0,1},{0,2},{1,0},{1,2}};
	int twelve[5][2]={{0,0},{0,1},{0,2},{1,1},{2,1}};
	int thirteen[5][2]={{0,0},{0,1},{0,2},{1,0},{2,0}};
	int fourteen[5][2]={{0,0},{0,1},{1,1},{1,2},{2,2}};
	int fifteen[5][2]={{0,0},{1,0},{1,1},{1,2},{2,2}};
	int sixteen[5][2]={{0,0},{1,0},{1,1},{1,2},{2,1}};
	int seventeen[5][2]={{0,1},{1,0},{1,1},{1,2},{2,1}};
	int eighteen[5][2]={{0,0},{0,1},{0,2},{0,3},{1,0}};
	int nineteen[5][2]={{0,0},{0,1},{0,2},{1,2},{1,3}};
	int twenty[5][2]={{0,0},{0,1},{0,2},{0,3},{1,1}};
	int twentyone[5][2]={{0,0},{0,1},{0,2},{0,3},{0,4}};
	int size[]={1,2,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5};
	int* block[21];
	block[0]=one;
	block[1]=two;
	block[2]=three;
	block[3]=four;
	block[4]=five;
	block[5]=six;
	block[6]=seven;
	block[7]=eight;
	block[8]=nine;
	block[9]=ten;
	block[10]=eleven;
	block[11]=twelve;
	block[12]=thirteen;
	block[13]=fourteen;
	block[14]=fifteen;
	block[15]=sixteen;
	block[16]=seventeen;
	block[17]=eighteen;
	block[18]=nineteen;
	block[19]=twenty;
	block[20]=twentyone;
	char *status="";  /*read the blokus board status*/
	char *map;
	char *path_status;
	char *left="";    /*read the left pattern*/
	char *path_left;    /*                need to change the color*/
	int first_time[]={4,4,9,9};
	
	if(strcmp(argv[1],"Red")==0){
		color='R';
		enemy_color='B';
	}
	else{
		color='B';
		enemy_color='R';
	}
	
	path_status=argv[2];
	path_left=argv[3];
	
	/*
	color='R';
	enemy_color='B';
	path_status="BlokusBoard.txt";
	path_left="LeftPattern_Color.txt";
	*/
	status=readFile(path_status);
	
	
	
	left=readFile(path_left);
	left_length=strlen(left);

	/*printf("%s \n","out of roop");
	
	printf("before:\n%s99 \n",status);
	printf("beforer:\n%s99\n",left);*/
	/*
	printf("%s \n",strncpy(map,status,1));
	printf("%s \n",strncpy(map,status+1,1));
	printf("%s \n",strncpy(map,status+2,1));
	printf("%s \n",strncpy(map,(status+first_time[0]*28+2*first_time[1]),1));
	*/
	for(i=0;i<2;i++){   /* the first put*/
		if(end!=0){
			break;
		}
		if(status[first_time[0]*28+2*first_time[1]]==color||status[first_time[2]*28+2*first_time[3]]==color){
				break;
			}
		if(status[first_time[2*i]*28+2*first_time[2*i+1]]=='N'){  /*the first time to put*/
		/*printf("%s \n","can test1 the 'N'");*/
			k=15; /*choose blocknumber 15 to put*/
			for(n=0;n<2;n++){    /* in this loop,it will reverse block 2 times(write in the underest loop)*/
				for(m=0;m<4;m++){  /* in this loop,it will rotate block 4 times(write in the underest loop)*/ 
						x_difference=first_time[2*i]-*(block[k-1]);   /*example: the block coordinate is (0,0),put in the map (6,4),*/
						y_difference=first_time[2*i+1]-*(block[k-1]+1); /*then x_difference=6-0=6,y_difference=4-0=4*/
						block_area=size[k-1];
						can_put_test=0;
						for(l=0;l<block_area;l++){  /* try to put the block into map*/
							x_coordinate=*(block[k-1]+2*l)+x_difference;  /*  x_coordinate is from 0 to 13*/
							y_coordinate=*(block[k-1]+2*l+1)+y_difference;  /*  y_coordinate is from 0 to 13*/
							if(x_coordinate<14&&x_coordinate>-1&&y_coordinate<14&&y_coordinate>-1){
								if(*(status+x_coordinate*28+2*y_coordinate)=='N'){
									int put_test2=second_put_test(x_coordinate,y_coordinate,status,color);
									if(put_test2==1){
										
										/*printf("block_number:%d  ",k);
										printf("xcoordinate:%d ycoordinate:%d \n",x_coordinate,y_coordinate);*/
										can_put_test++;
									}
								}
								else{
									break;
								}
							}
							else{
								break;
							}
						}
						/*printf("can_put_test:%d  \n",can_put_test);*/
						if(can_put_test==block_area){  /* can put the block in this map*/
							can_put_all(x_difference,y_difference,(block[k-1]),status,block_area,color); /* edit the map*/
							writeFile(path_status,status);
							/*printf("%s \n",status);*/
							edit_left(left,left_length,k);
							writeFile(path_left,left);
							/*printf("%s \n",left);*/
							end++;
							break;
							/*            write file let this block_number turn zero and write the map*/
						}
					
					for(l=0;l<size[k-1];l++){  /* rotate*/
						counterclock90((block[k-1]+2*l),(block[k-1]+2*l+1));
					}

				

				}
				if(end!=0){
					break;
				}
				for(l=0;l<size[k-1];l++){  /* reverse*/
					reverse(block[k-1]+2*l);
				}
				
			}
		}
	}
	/*printf("%s \n","out of roop2");*/

	
	
	/*while(end==0){*/
	int put_test1_1;
	int put_test1_2;
	for(i=0;i<14;i++){   /*this loop is try to catch enemy's edge's coordinate*/ /*i is row  (map)*/
		if(end!=0){  /*if it is first time to put*/
			break;
		}
		for(j=0;j<14;j++){  /*j is column  (map)*/
			/* *(status+i*28+2*j+1)   catch the 'R' 'N' 'B'*/
		
		
		
			if(status[i*28+2*j]=='N'){
				
				
				
				put_test1_1=first_put_test(i,j,status,color);
				put_test1_2=first_put_test(i,j,status,enemy_color);
				if(put_test1_1==1){
					edge_coordinate[2*edge_number]=i;
					edge_coordinate[2*edge_number+1]=j;
					edge_number+=1;	
				}
				if(put_test1_2==1){
					enemy_edge_coordinate[2*enemy_edge_number]=i;
					enemy_edge_coordinate[2*enemy_edge_number+1]=j;
					enemy_edge_number+=1;	
				}
				
			}
		}
	}
	/*printf("x:%d y:%d \n",i,j);*/
	/*printf("%s \n","out of roop3");*/
	for(k=21;k>0;k--){  /* choose the block_number  */
		if(end!=0){  /*if it is first time to put*/
			break;
		}
		for(n=0;n<2;n++){    /* in this loop,it will reverse block 1 times(write in the underest loop)*/
			for(m=0;m<4;m++){  /* in this loop,it will rotate block 4 times(write in the underest loop)*/
				/*if(left[left_length-1-2*(22-k)]==','){
					printf("left_length:%d",left_length);
					printf("error");
				}*/
				for(i=0;i<edge_number;i++){
					if(left[left_length-1-1-2*(21-k)]=='1'){  /*read the left_pattern file and find the non-used block*/ 
						/*printf("blocknumber:%d",k);*/
						for(o=0;o<size[k-1];o++){  /*choose the first location on the block*/
							x_difference=edge_coordinate[2*i]-*(block[k-1]+2*o);   /*example: the block coordinate is (0,0),put in the map (6,4),*/
							y_difference=edge_coordinate[2*i+1]-*(block[k-1]+2*o+1); /*then x_difference=6-0=6,y_difference=4-0=4*/
							block_area=size[k-1];
							can_put_test=0;
							block_enemy_test=0;
							try_near=0;
							future_way=0;
							/*block_number=-1;*/
							map=malloc(strlen(status)+1);
							/*printf("%s",status);*/
							strcpy(map,status);
							/*printf("%s",map);*/
							for(l=0;l<block_area;l++){  /* try to put the block into map*/
								x_coordinate=*(block[k-1]+2*l)+x_difference;  /*  x_coordinate is from 0 to 13*/
								y_coordinate=*(block[k-1]+2*l+1)+y_difference;  /*  y_coordinate is from 0 to 13*/
								if(x_coordinate<14&&x_coordinate>-1&&y_coordinate<14&&y_coordinate>-1){
									if(status[x_coordinate*28+2*y_coordinate]=='N'){
										int put_test2=second_put_test(x_coordinate,y_coordinate,status,color);
										if(put_test2==1){
											try_near+=near_block(x_coordinate,y_coordinate,status,enemy_color);
											
											/*printf("block_number:%d  ",k);
											printf("xcoordinate:%d ycoordinate:%d \n",x_coordinate,y_coordinate);*/
											can_put_test++;
											
											for(j=0;j<enemy_edge_number;j++){  //if the put_location can block enemy's edge
											
												if(x_coordinate==enemy_edge_coordinate[2*j]&&y_coordinate==enemy_edge_coordinate[2*j+1]){
												block_enemy_test++;
												}
											}
										}
									}
									else{
										break;
									}
								}
								else{
									break;
								}
							}
							if(can_put_test==block_area){  /* can put the block in this map*/
								if(block_enemy_test>=truely_put[6]){
									can_put_all(x_difference,y_difference,block[k-1],map,block_area,color); /* edit the map*/
									future_way=count_edge_number(map,color);
									if(future_way>=truely_put[7]){
										if(try_near>truely_put[8]){
											/*if(block_number!=-1&&(k==3||k==2||k==1)){
												continue;
											}
											block_number=k;*/
											truely_put[0]=x_difference;
											truely_put[1]=y_difference;
											truely_put[2]=k;
											truely_put[3]=block_area;
											truely_put[4]=m;
											truely_put[5]=n;
											truely_put[6]=block_enemy_test;
											truely_put[7]=future_way;
											truely_put[8]=try_near;
										}
									}
								
								}
								/*can_put_all(truely_put[0],truely_put[1],block[truely_put[2]-1],status,truely_put[3],color);
								/*can_put_all(x_difference,y_difference,(block[k-1]),status,block_area,color); /* edit the map*/
								/*edit_left(left,left_length,k);
								writeFile(path_status,status);
								writeFile(path_left,left);
								end++;
								
								/*printf("after:\n%s \n",status);
								printf("after:\n%s \n",left);*/
								/*            write file let this block_number turn zero and write the map*/
							}
							/*if(end!=0){
								break;
							}*/
						}
						/*if(end!=0){
							break;
						}*/
					}
					/*if(end!=0){
						break;
					}*/
					
				}
				/*if(end!=0){
					break;
				}*/
				for(l=0;l<size[k-1];l++){  /* rotate*/
						/*printf("(%d,",*(block[k-1]+2*l));
						printf("%d)",*(block[k-1]+2*l+1));*/
						
						counterclock90((block[k-1]+2*l),(block[k-1]+2*l+1));
						/*printf("After_rotate:(%d,",*(block[k-1]+2*l));
						printf("%d)\n",*(block[k-1]+2*l+1));*/
					}
			
	
			}
			/*if(end!=0){
				break;
			}*/
			for(l=0;l<size[k-1];l++){  /* reverse*/
				reverse(block[k-1]+2*l);
			}
			
		}
	}
	/*printf("%s \n","out of roop4");*/
	
	if(truely_put[6]!=-1){
		for(n=0;n<truely_put[5];n++){
			for(l=0;l<size[truely_put[2]-1];l++){  /* rotate*/
				reverse((block[truely_put[2]-1]+2*l));
			}
		}
		for(m=0;m<truely_put[4];m++){
			for(l=0;l<size[truely_put[2]-1];l++){  /* rotate*/
			counterclock90((block[truely_put[2]-1]+2*l),(block[truely_put[2]-1]+2*l+1));
			}
		}
		can_put_all(truely_put[0],truely_put[1],block[truely_put[2]-1],status,truely_put[3],color); /* edit the map*/
		edit_left(left,left_length,truely_put[2]);
		writeFile(path_status,status);
		writeFile(path_left,left);
	}
	
	
	
	/*}*/
	/*
	printf("%d  ",block[3]);
	printf("%d  ",&four[0][0]);
	printf("%d  ",&four[0][1]);
	printf("%d  ",&four[1][0]);
	printf("%d  \n",&four[1][1]);
	printf("%d  ",block[4]);
	printf("%d  ",&five[0][0]);
	printf("\n");
	for(i=0;i<21;i++){
		for(j=0;j<size[i];j++){
			printf("(%d,",*(block[i]+2*j));
			printf("%d)",*(block[i]+2*j+1));
			printf("i:%d j:%d ",i,j);
			printf("%d ",block[i]+j);   
		}
		printf("\n");
	}
		*/
	
}
