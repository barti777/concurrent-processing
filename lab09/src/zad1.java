import java.util.Random;

class threadZad1 extends Thread {

    private int number;
    private int start;
    private int end;
    private int[] array1;
    private int[] array2;
    private int[] arrayResults;

    threadZad1(
            int number,
            int[] array1,
            int[] array2,
            int[] arrayResults,
            int start,
            int end
    ) {
        this.number = number;
        this.array1 = array1;
        this.array2 = array2;
        this.arrayResults = arrayResults;
        this.start = start;
        this.end = end;
    }

    public void run() {
        System.out.println("Watek " + number + " ma zakres od " + start + " do " + end);

        for (int i = start; i <= end; ++i) {
            this.arrayResults[i] = this.array1[i] + this.array2[i];
        }
    }
}

public class zad1 {

    static private final Random random = new Random();

    private static final int n = 42;

    public static void main(String[] args) {
        func();
    }

    private static void func() {
        int[] vector_one = new int[n];
        int[] vector_two = new int[n];

        for (int i = 0; i < n; ++i) {
            vector_one[i] = random.nextInt(49);
            vector_two[i] = random.nextInt(49);
        }

        int[] vector_results = new int[n];
        int numThreads = 10;

        threadZad1[] NewThr = new threadZad1[numThreads];

        int division = (n - 1) / numThreads;

        division += 1;

        int start = 0;
        int end;

        for (int i = 0; i < numThreads; ++i) {
            end = start + division;
            if (end >= n) {
                end = n - 1;
            }
            (NewThr[i] = new threadZad1(i, vector_one, vector_two, vector_results, start, end)).start();
            start = end;
        }

        for (int i = 0; i < numThreads; ++i) {
            try {
                NewThr[i].join();
            } catch (InterruptedException e) {
                System.out.println(e.getMessage());
            }
        }

        System.out.print("\nWynik: ");
        for (int i = 0; i < n; ++i) {

            if (i % 8 == 0) {
                System.out.println();
            }
            System.out.print("(" + vector_one[i] + " + " + vector_two[i] + " = " + vector_results[i] + ") ");
        }
    }
}
