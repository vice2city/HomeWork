#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>

#define HASH_LEN 50
#define ADDRESS_LEN 50
#define NAME_LEN 20
#define BARREL_LEN 5

typedef unsigned long long tel; //电话号码定义

typedef struct Telnumber
{
    tel number;
    struct Telnumber *next;
    struct Contract *entry;
}telnumber; //电话号码结构体

typedef struct Contract
{
    char name[NAME_LEN];
    telnumber *number;
    char address[ADDRESS_LEN];
    struct Contract *next;
}contract; //联系人结构体

typedef struct Barrel {
    telnumber node[BARREL_LEN];
    int count;
}barrel; //排序桶结构体

static contract book[HASH_LEN]; //电话本全局变量

void initialContract(){ //初始化联系人结构体
    int i;
    for(i=0;i<HASH_LEN;i++){
        strcpy(book[i].address, "\0");
        strcpy(book[i].name, "\0");
        book[i].next = NULL;
        book[i].number = NULL;
    };
};

int cmpfunc(const void* a, const void* b) //快速排序函数
{
    return ((*(telnumber*)a).number - (*(telnumber*)b).number);
}

int binarySearch(telnumber node[], int n, tel key) { //二分查找
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        tel midVal = node[mid].number;
        if (midVal < key)
            low = mid + 1;
        else if (midVal > key)
            high = mid - 1;
        else
            return mid;
    }
    return -1;
}

barrel* sortEntry(int needDisplay) { //排序功能（桶排序）
    int i, k, pos = 0;
    tel max = 0, min = 0, bucket_num;

    for (i = 0; i < HASH_LEN; i++) {
        contract* cp = book[i].next;
        for (; cp != NULL; cp = cp->next)
        {
            telnumber* tp = cp->number;
            for (; tp != NULL; tp = tp->next){
            if (tp->number > max)
                max = tp->number;
            if (min == 0 || tp->number < min)
                min = tp->number;
            }
        }
    }

    if (min == max && min == 0){
        printf("记录为空。\n");
        return NULL;
        }

    bucket_num = (max - min + 1) / BARREL_LEN + 1;
    barrel* pBarrel = (barrel*)malloc(sizeof(barrel) * bucket_num);
    for (i = 0; i < bucket_num; i++) {
        (pBarrel + i)->count = 0;
    }

    for (i = 0; i < HASH_LEN; i++) {
        contract* cp = book[i].next;
        for (; cp != NULL; cp = cp->next)
        {
            telnumber* tp = cp->number;
            for (; tp != NULL; tp = tp->next) {
                k = (tp->number - min + 1) / BARREL_LEN;
                (pBarrel + k)->node[(pBarrel + k)->count] = *tp;
                (pBarrel + k)->count++;
            }
        }
    }

    if (needDisplay)
    printf("序号\t姓名\t电话\t地址\n");

    for (i = 0; i < bucket_num; i++) {
        qsort((pBarrel + i)->node, (pBarrel + i)->count, sizeof(telnumber), cmpfunc);

        if (needDisplay)
        for (k = 0; k < (pBarrel + i)->count; k++) {
            pos++;
            telnumber t = (pBarrel + i)->node[k];
            printf("（%d） %s %lld %s\n", pos, t.entry->name, t.number, t.entry->address);
        }
    }
    return pBarrel;
}

int CountHash(char *s){ //计算Hash值
    int i;
    unsigned int ascii=0;
    for (i=0;*(s+i)!='\0';i++)
    ascii += toascii(*(s+i));
    return ascii % HASH_LEN;
};

contract *Lookup_Name(char *name){ //查找姓名
    int hashValue = CountHash(name);
    contract *cp = &book[hashValue];
    contract *temp = NULL;
    for (; cp != NULL; cp = cp->next)
	{
        temp = cp;
		if (!strcmp(cp->name, name)) //出现重名
        return cp;
	}
    return temp;
}

telnumber *Lookup_Tel(tel number, contract *entry){ //查找号码（同一联系人）
    telnumber *tp = entry->number;
    telnumber *temp = NULL;
    for (; tp != NULL; tp = tp->next)
	{
        temp = tp;
		if (tp ->number == number) //出现重复号码
        return tp;
	}
    return temp;
};

telnumber *CreateTel(tel number, contract *entry){ //创建号码项
    telnumber *tp = (telnumber*)malloc(sizeof(telnumber));
    tp->number = number;
    tp->next = NULL;
    tp->entry = entry;
    return tp;
};

contract* CreateContract(char name[]) { //创建联系人项
    contract* cp = (contract*)malloc(sizeof(contract));
    tel number = 0;

    strcpy(cp->name, name);
    printf("请输入地址（少于%d个字母）：", ADDRESS_LEN);
    gets(cp->address);
    printf("请输入号码：");
    scanf("%lld", &number);
    cp->number = CreateTel(number, cp);
    cp->next = NULL;
    return cp;
};

int CreateEntry(){ //创建条目功能
    char name[NAME_LEN] = "\0";
    tel number = 0;
    int i=0;

    printf("请输入姓名（少于%d个字母）：", NAME_LEN);
    while (!strcmp("\0", name))
    gets(name);
    contract *cp = Lookup_Name(name);

    if(!strcmp(cp->name, name)){ //已有该联系人
        printf("已存在此联系人，请输入要新增的号码： ");
        scanf("%lld", &number);
        telnumber *tp = Lookup_Tel(number, cp);
        if(tp->number == number){ //存在该号码
            printf("已有重复号码，创建失败。\n");
            return 0;
        }else{ //新建同名号码
            tp->next = CreateTel(number, cp);
            printf("新增号码成功。\n");
            return 1;
        }
    }
    else {
        cp->next = CreateContract(name);
        printf("新建联系人成功。\n");
        return 1;
    }
};

int RemoveEntry(contract* cp) {
    telnumber* tp = cp->number;
    telnumber* temp_tel = NULL;
    while (tp != NULL) {
        temp_tel = tp;
        tp = tp->next;
        free(temp_tel);
    }
    int hashValue = CountHash(cp->name);
    contract* temp = &book[hashValue];
    for (; temp->next != NULL; temp = temp->next)
    {
        if (temp->next == cp)
            break;
    }
    temp->next = cp->next;
    free(cp);
    printf("\n删除联系人成功。\n");
}

int DisplayEntry(contract* cp){
    telnumber* tp = cp->number;
    int i = 0;
    printf("姓名： %s\n地址： %s\n", cp->name, cp->address);
    for (; tp != NULL; tp = tp->next)
        printf("电话（%d）： %lld\n", ++i, tp->number);
}

int EditTel(contract* cp) { //编辑联系人号码
    telnumber* tp = cp->number;
    telnumber* temp = NULL;
    int i = 0, k = 0, opt = 0, num;
    tel number = 0;

    while (opt != 4) {
        tp = cp->number;
        for (; tp != NULL; tp = tp->next)
            printf("序号（%d）： %lld\n", ++i, tp->number);
        printf("-------------------------\n");
        num = i;
        printf("1.添加号码\n2.编辑号码\n3.删除号码\n4.返回上一级\n");
        printf("-------------------------\n");
        printf("请输入需要进行的操作：");
        scanf("%d", &opt);
        printf("-------------------------\n");
        switch (opt) {
        case 1:
            printf("请输入要添加的号码：");
            scanf("%lld", &number);
            if (Lookup_Tel(number, cp)->number == number) {
                printf("已有重复号码，创建失败。\n");
                break;
            }
            for (i = 0, tp = cp->number; i < num - 1; i++)
                tp = tp->next;
            tp->next = CreateTel(number, cp);
            num++;
            printf("\n添加号码成功。\n");
            break;
        case 2:
            printf("请输入要编辑的号码序号：");
            scanf("%d", &k);
            if (k<1 || k>num) {
                printf("\n不存在该号码。\n");
                break;
            }
            else {
                printf("请输入修改后的号码：");
                scanf("%lld", &number);
                for (i = 0, tp = cp->number; i < k - 1; i++)
                    tp = tp->next;
                tp->number = number;
                printf("\n修改号码成功。\n");
            }
            break;
        case 3:
            printf("请输入要删除的号码序号：");
            scanf("%d", &k);
            if (k<1 || k>num) {
                printf("\n不存在该号码。\n");
                break;
            }
            else {
                for (i = 0, tp = cp->number; i < k - 2; i++)
                    tp = tp->next;
                temp = tp->next;
                tp->next = tp->next->next;
                free(temp);
                num--;
                printf("\n删除号码成功。\n");
            }
            break;
        case 4:
            break;
        default:
            printf("请输入正确的操作。\n");
            break;
        }
    }
}

void CustomOpt(contract* cp) { //针对单个条目的功能
    int opt = 0;
    char name[NAME_LEN] = "\0";
    char address[ADDRESS_LEN] = "\0";
    printf("-------------------------\n");
    
    while (opt != 5) {
        printf("1.修改姓名\n2.修改地址\n3.编辑电话号码\n4.删除联系人\n5.返回上一级\n");
        printf("-------------------------\n");
        printf("请输入需要进行的操作：");
        scanf("%d", &opt);
        printf("-------------------------\n");
        switch (opt){
        case 1:
            printf("请输入修改后的姓名（少于%d个字母）：", NAME_LEN);
            while (!strcmp("\0", name))
            gets(name);
            strcpy(cp->name, name);
            printf("\n修改姓名成功。\n");
            break;
        case 2:
            printf("请输入修改后的地址（少于%d个字母）：", ADDRESS_LEN);
            while (!strcmp("\0", address))
            gets(address);
            strcpy(cp->address, address);
            printf("\n修改地址成功。\n");
            break;
        case 3:
            EditTel(cp);
            break;
        case 4:
            printf("确定要删除吗（1：确认 other：取消）：");
            scanf("%d", &opt);
            if (opt == 1)
                RemoveEntry(cp);
            break;
        case 5:
            break;
        default:
            printf("请输入正确的操作。\n");
            break;
        }
    }
}

int Search_Tel() { //按号码查找功能
    barrel* temp = sortEntry(0);
    if (temp == NULL)
        return 0;

    barrel* pBarrel = temp;
    tel number = 0;
    printf("请输入要查询的号码： ");
    scanf("%lld", &number);

    while (pBarrel->node[pBarrel->count-1].number < number || !pBarrel->count)
        pBarrel++;
    int key = binarySearch(pBarrel->node, pBarrel->count, number);
    if (key == -1){
        printf("未找到记录。\n");
        return 0;
    }else {
        telnumber t = pBarrel->node[key];
        printf("找到记录：\n");
        DisplayEntry(t.entry);
        CustomOpt(t.entry);
        return 1;
    }
}

int Search_Name(){ //按姓名查找功能
    char name[NAME_LEN] = "\0";
    printf("请输入姓名（少于%d个字母）：\n", NAME_LEN);
    while (!strcmp("\0", name))
    gets(name);
    contract *cp = Lookup_Name(name);

    if(strcmp(cp->name, "\0")&&!strcmp(cp->name, name)){ //找到该联系人
        printf("找到记录：\n");
        DisplayEntry(cp);
        CustomOpt(cp);
        return 1;
    }else{
        printf("未找到记录。\n");
        return 0;
    }
};

int main(void){ //主函数
    int opt = 0;
    initialContract();
    printf("欢迎使用电话簿。\n");
    printf("-------------------------\n");

    while (opt != 5)
    {
     printf("1.顺序输出联系人列表\n2.新建联系人\n3.按姓名查找联系人\n4.按号码查找联系人\n5.退出系统\n");
     printf("-------------------------\n");
     printf("请输入你所需要进行的操作：");
     scanf("%d", &opt);
     printf("-------------------------\n");

     switch (opt)
        {
        case 1:
            free(sortEntry(1));
            printf("-------------------------\n");
            break;
        case 2:
            CreateEntry();
            printf("-------------------------\n");
            break;
        case 3:
            Search_Name();
            printf("-------------------------\n");
            break;
        case 4:
            Search_Tel();
            printf("-------------------------\n");
            break;
        case 5:
            printf("欢迎你再次使用。\n");
            break;
        default:
            printf("请输入正确的操作。\n");
            break;
        }
    }
    
    return 0;
}