#include <omp.h>

#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

class MyProg {
   private:
      char *array;

      int index;

      int nThreads,
          nTimes,
          nChunks,
          nTotal;

      bool wMutex;

   public:
      MyProg(int t, int s, int c, int wm) : nThreads(t), nTimes(s), nChunks(c), wMutex(wm) {
         nTotal = nThreads * nTimes;

         index  = 0;
         array  = new char[nTotal];
         fill(array, array + nTotal, '-');

         omp_set_num_threads(nThreads); // Use nThreads for all consecutive parallel regions
      }

      ~MyProg() {
         delete[] array;
      }

      void run(int t) {
         switch (t) {
            case 1:
               runStatic();
               break;
            case 2:
               runStaticWithChunck();
               break;
            case 3:
               runDynamic();
               break;
            case 4:
               runDynamicWithChunk();
               break;
            case 5:
               runGuided();
               break;
            case 6:
               runGuidedWithChunk();
               break;
            case 7:
               runRuntime();
               break;
            case 8:
               runAuto();
               break;
         }

         printInfos();
      }
   
   private:
      void runStatic() {
         #pragma omp parallel for schedule(static) shared(array, index)
         for (int i = 0 ; i < nTotal ; i++) {
            addChar('A' + omp_get_thread_num());
         }
      }

      void runStaticWithChunck() {
         #pragma omp parallel for schedule(static, nChunks) shared(array, index)
         for (int i = 0 ; i < nTotal ; i++) {
            addChar('A' + omp_get_thread_num());
         }
      }

      void runDynamic() {
         #pragma omp parallel for schedule(dynamic) shared(array, index)
         for (int i = 0 ; i < nTotal ; i++) {
            addChar('A' + omp_get_thread_num());
         }
      }

      void runDynamicWithChunk() {
         #pragma omp parallel for schedule(dynamic, nChunks) shared(array, index)
         for (int i = 0 ; i < nTotal ; i++) {
            addChar('A' + omp_get_thread_num());
         }
      }

      void runGuided() {
         #pragma omp parallel for schedule(guided) shared(array, index)
         for (int i = 0 ; i < nTotal ; i++) {
            addChar('A' + omp_get_thread_num());
         }
      }

      void runGuidedWithChunk() {
         #pragma omp parallel for schedule(guided, nChunks) shared(array, index)
         for (int i = 0 ; i < nTotal ; i++) {
            addChar('A' + omp_get_thread_num());
         }
      }

      void runRuntime() {
         #pragma omp parallel for schedule(runtime) shared(array, index)
         for (int i = 0 ; i < nTotal ; i++) {
            addChar('A' + omp_get_thread_num());
         }
      }

      void runAuto() {
         #pragma omp parallel for schedule(auto) shared(array, index)
         for (int i = 0 ; i < nTotal ; i++) {
            addChar('A' + omp_get_thread_num());
         }
      }

      void addChar(char c) {
         if(wMutex) {
            #pragma omp critical
            {
               array[index] = c;
               spendSomeTime();
               index++;
            }
         }
         else {
            array[index] = c;
            spendSomeTime();
            index++;
         }
      }

      void spendSomeTime() {
         for (int i = 0; i < 10000; i++) {
            for (int j = 0; j < 100; j++) {
               // These loops shouldn't be removed by the compiler
            }
         }
      }

      int countOccurrences(char c) {
         return count(array, array + nTotal, c);
      }

      string toString() {
         return string(array, nTotal);
      }

      void printInfos() {
         cout << toString() << endl;
         for(int i = 0 ; i < nThreads ; i++)
            cout << (char)('A' + i) << "="
                     << countOccurrences('A' + i) << " ";
         cout << endl;
      }
};


int main(int argc, char **argv) {

   int nThreads, nTimes, nChunk;

   if(argc < 4) {
      cout << "Try: " << argv[0] << " <num threads> <num times> <num chunk>" << endl;
      return EIO;
   }

   nThreads = stoi(argv[1]);
   nTimes   = stoi(argv[2]);
   nChunk   = stoi(argv[3]);

   cout << "\nNThreads: " << nThreads 
        << " - NTotal: " << (nThreads * nTimes)
        << " - NChunk: " << nChunk  << endl;
   cout << "\n===========================" << endl;
   cout << "===   Cases With Lock   ===" << endl;
   cout << "===========================" << endl;
   cout << "Case 1: Static" << endl;
   MyProg m1(nThreads, nTimes, nChunk, true);
   m1.run(1);

   cout << "\nCase 2: Static && Chunk" << endl;
   MyProg m2(nThreads, nTimes, nChunk, true);
   m2.run(2);

   cout << "\nCase 3: Dynamic" << endl;
   MyProg m3(nThreads, nTimes, nChunk, true);
   m3.run(3);

   cout << "\nCase 4: Dynamic && Chunk" << endl;
   MyProg m4(nThreads, nTimes, nChunk, true);
   m4.run(4);

   cout << "\nCase 5: Guided" << endl;
   MyProg m5(nThreads, nTimes, nChunk, true);
   m5.run(5);

   cout << "\nCase 6: Guided && Chunk" << endl;
   MyProg m6(nThreads, nTimes, nChunk, true);
   m6.run(6);

   cout << "\nCase 7: Runtime" << endl;
   MyProg m7(nThreads, nTimes, nChunk, true);
   m7.run(7);

   cout << "\nCase 8: Auto" << endl;
   MyProg m8(nThreads, nTimes, nChunk, true);
   m8.run(8);

   cout << "\n===========================" << endl;
   cout << "=== Cases Without Lock  ===" << endl;
   cout << "===========================" << endl;
   cout << "Case 1: Static" << endl;
   MyProg wom1(nThreads, nTimes, nChunk, false);
   wom1.run(1);

   cout << "\nCase 2: Static && Chunk" << endl;
   MyProg wom2(nThreads, nTimes, nChunk, false);
   wom2.run(2);

   cout << "\nCase 3: Dynamic" << endl;
   MyProg wom3(nThreads, nTimes, nChunk, false);
   wom3.run(3);

   cout << "\nCase 4: Dynamic && Chunk" << endl;
   MyProg wom4(nThreads, nTimes, nChunk, false);
   wom4.run(4);

   cout << "\nCase 5: Guided" << endl;
   MyProg wom5(nThreads, nTimes, nChunk, false);
   wom5.run(5);

   cout << "\nCase 6: Guided && Chunk" << endl;
   MyProg wom6(nThreads, nTimes, nChunk, false);
   wom6.run(6);

   cout << "\nCase 7: Runtime" << endl;
   MyProg wom7(nThreads, nTimes, nChunk, false);
   wom7.run(7);

   cout << "\nCase 8: Auto" << endl;
   MyProg wom8(nThreads, nTimes, nChunk, false);
   wom8.run(8);

   return 0;
}