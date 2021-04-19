//
//  main.c
//  OS_Project2
//
//  Created by JongEun Kim on 2021/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char * argv[]) {
    
    int p_num = 0; // 프로세스 개수
    int r_type_num = 0; // 리소스 종류 개수
    int tmp = 0; // 임시
    int blockcheck = 0; // 0 = block, 1 =  unblock
    int deadlockcheck = 0; // 0 = deadlock, 1 = non-deadlock
    int arrequalcheck = 0; // 0 = unequal, 1 = equal
    
    FILE *fp;
    fp = fopen("input.txt", "r"); // input.txt open -> fp
    
    fscanf(fp, "%d", &p_num);
    fscanf(fp, "%d", &r_type_num);
    
    int* arr_r_unit;
    arr_r_unit = (int *)malloc(sizeof(int)*r_type_num);
    memset(arr_r_unit, 0, r_type_num*sizeof(int));
    //arr_r_unit[] -> 리소스별 리소스 개수 1차원 배열
    
    int* arr_r_remain;
    arr_r_remain = (int *)malloc(sizeof(int)*r_type_num);
    memset(arr_r_remain, 0, r_type_num*sizeof(int));
    //arr_r_remain[] -> 리소스별 남은 리소스 개수 1차원 배열
    
    int* arr_p_block;
    arr_p_block = (int *)malloc(sizeof(int)*r_type_num);
    memset(arr_p_block, 0, sizeof(int)*r_type_num);
    //arr_p_block[] -> 프로세스 block 여부 확인 , 0 -> block, 1 -> unblock
    
    int* arr_p_block_former;
    arr_p_block_former = (int *)malloc(sizeof(int)*r_type_num);
    memset(arr_p_block_former, 0, sizeof(int)*r_type_num);
    //arr_p_block_former[] -> 이전 단계의 arr_p_block[]
    
    for (int i = 0; i<r_type_num; i++) {
        fscanf(fp, "%d", &tmp);
        arr_r_unit[i] = tmp;
    }
    //arr_r_unit[] 입력
    
    tmp = 0;
    
    int **arr_alloc;
    arr_alloc = (int**) malloc ( sizeof(int*) * p_num );
    for(int i=0; i<p_num; i++){
        arr_alloc[i] = (int*) malloc ( sizeof(int) * r_type_num );
    }
    //arr_alloc[][] -> alloc 된 리소스 2차원배열
    
    int **arr_req;
    arr_req = (int**) malloc ( sizeof(int*) * p_num );
    for(int i=0; i<p_num; i++){
        arr_req[i] = (int*) malloc ( sizeof(int) * r_type_num );
    }
    //arr_req[][] -> req하는 리소스 2차원배열
    
    for (int i= 0; i<p_num; i++) {
        for (int j = 0; j<r_type_num; j++) {
            fscanf(fp, "%d", &tmp);
            arr_alloc[i][j] = tmp;
        }
    }
    //arr_alloc[][] 입력
    
    tmp = 0;
    
    for (int i= 0; i<p_num; i++) {
        for (int j = 0; j<r_type_num; j++) {
            fscanf(fp, "%d", &tmp);
            arr_req[i][j] = tmp;
        }
    }
    //arr_req[][] 입력
    
/*
    //input test
    printf("%d %d ",p_num,r_type_num);
    
    for (int i = 0; i<r_type_num; i++) {
        printf("%d ",arr_r_unit[i]);
    }
    printf("\n");
    
    
    for (int i= 0; i<p_num; i++) {
        for (int j = 0; j<r_type_num; j++) {
            printf("%d ",arr_alloc[i][j]);
        }
        printf("\n");
    }
    
    for (int i= 0; i<p_num; i++) {
        for (int j = 0; j<r_type_num; j++) {
            printf("%d ",arr_req[i][j]);
        }
        printf("\n");
    }
    //input test
     */
    
    tmp = 0;
    
    for(int k = 0; k<p_num; k++) {
        printf("\nProcess graph reduction Round : %d\n",k+1);
       /*
        printf("arr_r_unit\n");
        for (int i = 0; i<r_type_num; i++) {
            printf("%d ",arr_r_unit[i]);
        }
        printf("\n");
        */
        
        for (int i = 0; i<r_type_num; i++) {
            arr_r_remain[i] = arr_r_unit[i];
            //printf("arr_r_remain[%d](%d) = arr_r_unit[%d](%d)\n",i,arr_r_remain[i],i,arr_r_unit[i]);
            for (int j = 0; j<p_num; j++) {
                arr_r_remain[i] = arr_r_remain[i] - arr_alloc[j][i];
                //printf("arr_r_remain[%d](%d) = arr_r_remain[%d](%d) - arr_alloc[%d][%d](%d)\n",i,arr_r_remain[i],i,arr_r_remain[i],i,j,arr_alloc[i][j]);
            }
        } // calculate arr_r_remain[]
        
        /*
        printf("arr_r_remain\n");
        for (int d = 0; d<r_type_num; d++) {
            printf("%d ",arr_r_remain[d]);
        }
        printf("\n");
         */
        
        
        for (int i = 0; i<p_num; i++) {
            for (int j = 0; j<r_type_num; j++) {
                if (arr_r_remain[j] >= arr_req[i][j]) {
                    blockcheck = 1;
                }
                else{
                    blockcheck = 0;
                    break;
                }
            }// unblock -> RET: blockcheck=1, block -> RET: blockcheck=0
            
            if (blockcheck == 1 && arr_p_block[i]==0) {
                printf("P%d is unblock state!\n",i+1);
                for (int j = 0; j<r_type_num; j++) {
                    arr_r_remain[j] = arr_r_remain[j] + arr_alloc[i][j];
                    arr_req[i][j] = 0;
                    arr_alloc[i][j] = 0;
                }
                printf("P%d is return all resources!\n",i+1);
                arr_p_block[i] = 1;
                break;
            }
            else if (blockcheck == 0 && arr_p_block[i]==0){
                printf("P%d is block state!\n",i+1);
            }
        }
        
        for (int y = 0; y<p_num; y++) {
            if (arr_p_block_former[y] == arr_p_block[y]) {
                arrequalcheck = 1;
            }
            else{
                arrequalcheck = 0;
                break;
            }
        }
        
        if (arrequalcheck == 1) {
            printf("All process are in block state!\n");
            break;
        }
        
        for (int y = 0; y<p_num; y++) {
            arr_p_block_former[y] = arr_p_block[y];
        }
    }
    
    for (int i = 0; i<p_num; i++) {
        if (arr_p_block[i] == 1) {
            deadlockcheck = 1;
        }
        else{
            deadlockcheck = 0;
            break;
        }
    }
    
    if (deadlockcheck == 0) {
        printf("\nfinal state : deadlock!\n");
        printf("Deadlocked process list : ");
        for (int i = 0; i<p_num; i++) {
            if (arr_p_block[i] == 0) {
                printf("P%d ",i+1);
            }
        }
        printf("\n");
    }
    else if (deadlockcheck == 1){
        printf("\nfinal state : non-deadlock!\n");
    }
    
    
    /*
    //input test
    printf("%d %d ",p_num,r_type_num);
    
    for (int i = 0; i<r_type_num; i++) {
        printf("%d ",arr_r_unit[i]);
    }
    printf("\n");
    
    
    for (int i= 0; i<p_num; i++) {
        for (int j = 0; j<r_type_num; j++) {
            printf("%d ",arr_alloc[i][j]);
        }
        printf("\n");
    }
    
    for (int i= 0; i<p_num; i++) {
        for (int j = 0; j<r_type_num; j++) {
            printf("%d ",arr_req[i][j]);
        }
        printf("\n");
    }
    //input test
     */

    
    
    
    
    
    

    
    
    
    
    
    
    
    
    
    return 0;
}
