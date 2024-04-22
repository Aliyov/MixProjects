//Support xətti üçün 2 versiya yazmışam birinci sadədi ancaq 1 aralığı çıxardır. İkinci biraz qəlizdi ən çox 
//rastlanan aralıqları tezliyi ilə birgə çıxardır.

//CSV faylını oxumaq üçün funksiya yazmışam amma tamamlanmayıb deyə üstündə işləmək olmur. Funksiya ancaq oxuya bilir.
//Kod Random aralıq məntiqi ilə işləyir 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define R_SIMULATION_NUM 21 //Simulasiya sayı
#define MAX_CHANGE 600 //Qiymətin max aşağı və ya yuxarı qalxma limiti
#define WINDOW_SIZE 250 //Nə qədər aralığın (blokun) support xətti olması
#define MAX_LINE_LENGTH 500 //CSV oxuayanda sətr başına max oxuma


void read_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    FILE *fp2 = fopen("csv.txt","w");
    if (file == NULL) {
        perror("File error");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, file);

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        
        char *token = strtok(line, ","); 

        while (token != NULL) {
            fprintf(fp2,"%s\n", token);
            token = strtok(NULL, ","); 
        }
        fprintf(fp2,"\n"); 
    }

    fclose(file);
    fclose(fp2);
}


//MA hesablanmasi
double MA(int *prices, int current_index, int window_size){
    /*
    if (current_index < window_size) {
        return 0; 
    }
    */


    int count = 0;
    double sum = 0.0;

    for (int i = current_index; i > current_index - window_size && i >= 0; i--) {
        sum += prices[i];
        count++;
    }
    // Sıfıra bölmə
    if (count > 0) {
        return sum / count;  
    } else {
        return 0; 
    }

}

// Aralığın tapılması--45000 ilə 45250 aralığındakı qiymətləri bir ədəd kimi götürmək üçün 250yə bölüb təzdən 250 vurmaq lazımdı 
int find_bucket(int number) {
    return (number / WINDOW_SIZE) * WINDOW_SIZE;
}

//VERSİYA 1. Support xəttinin tapılması amma burda tək bir support xətti tapırıq. İkinci versiyada bir çox support xətti tapırıq
int find_support_line(int *prices, int size){
    int freq_map[100000] = {0};  // Max qiymətin hara qədər qalxacağına görə tənzimlənir
    int max_freq = 0;
    int most_freq_bucket = 0;

    for (int i = 1; i < size; i++) {
        int bucket = find_bucket(prices[i]);
        int index = bucket / WINDOW_SIZE;

        if (++freq_map[index] > max_freq) {
            max_freq = freq_map[index];
            most_freq_bucket = bucket;
        }
    }
    return most_freq_bucket;
}

void support_or_resistance(int line, int avg,FILE *fp){
    if(avg>line){
        fprintf(fp,"Possible Support Line\n");
    }else{
        fprintf(fp,"Possible Resistance Line\n");
    }
}


//VERSIYA 2. Tezliklərlə birgə support xəttinin tapılması.

typedef struct {
    int bucket;
    int frequency;
} BucketFreq;

//Tezlikləri müqayisə etmək
int compare_buckets(const void *a, const void *b) {
    return ((BucketFreq*)b)->frequency - ((BucketFreq*)a)->frequency;
}

void find_support_lines(int *prices, int size, int top_n, FILE *fp) {
    int freq_map[100000] = {0};
    BucketFreq *top_buckets = malloc(sizeof(BucketFreq) * 100000);
    if (!top_buckets) {
        fprintf(stderr, "Mem error\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        int bucket = find_bucket(prices[i]);
        int index = bucket / WINDOW_SIZE;
        freq_map[index]++;
    }

    // Ən çox (top_n) structların içinin doldurulması
    int count = 0;
    for (int i = 0; i < 100000; i++) {
        if (freq_map[i] > 0) {
            top_buckets[count].bucket = i * WINDOW_SIZE;
            top_buckets[count].frequency = freq_map[i];
            count++;
        }
    }

    qsort(top_buckets, count, sizeof(BucketFreq), compare_buckets);

    fprintf(fp,"Top %d most frequent buckets:\n", top_n);
    for (int i = 0; i < top_n && i < count; i++) {
        fprintf(fp,"Bucket: %d, Frequency: %d\n", top_buckets[i].bucket, top_buckets[i].frequency);
    }

    free(top_buckets);
}


// Ba burda poxu çıxdı funksiyası 
void randomity(){
    FILE *fp = fopen("randomity.txt","w");
    if (!fp) {
        perror("Failed error");
        return;
    }

    int prices[R_SIMULATION_NUM];
    prices[0] = 45000; //45mindən başlayır aralıq yoxdu yuxarıda gedə bilər aşağıda
    srand(time(NULL));
    double maAVG; 
    for(int i = 1; i < R_SIMULATION_NUM; i++){
        int change = (rand() % (MAX_CHANGE * 2 + 1)) - MAX_CHANGE;
        prices[i] = prices[i-1] + change;
        
        maAVG = MA(prices, i, 20); // MA hesablayan funksiya
        int percentage = ((prices[i] * 100) / maAVG) - 100; //MA ya görə hal hazırki qiymətin dəyişmə faizi
        fprintf(fp, "Price %d: %d, Change: %d, Percentage: %d%%, MA Average: %.2f\n", 
                i, prices[i], change, percentage, maAVG);
    }

    int support_line = find_support_line(prices, R_SIMULATION_NUM);
    fprintf(fp, "Support line: %d\n", support_line);
    support_or_resistance(support_line, maAVG,fp);
    find_support_lines(prices, R_SIMULATION_NUM, 5,fp);
    fclose(fp);
}

int main(int argc, char *argv[]){
    randomity();
    //const char *filename = "BTC.csv";
    //read_csv(filename);
    return 0;
}

