class threadZad3 extends Thread {

    private int number;
    private int[] array;

    threadZad3(int number, int[] array) {
        this.number = number;

        this.array = array;
    }

    public void run() {
        for (int i = number; i < array.length; i++) {
            if (number < 2) {
                array[i] = 0;
                break;
            } else if (array[i] % number == 0 && array[i] != number) {
                array[i] = 0;
            }
        }
    }
}

public class zad3 {

    public static void main(String[] args) {
        func();
    }

    private static final int size = 2000;

    private static void func() {
        int number = 1;
        int numThreads = 10;
        int[] array = new int[size];
        for (int i = 0; i < size; i++) {
            array[i] = i;
        }
        threadZad3[] newThread = new threadZad3[numThreads];
        for (int i = 0; ; i++) {
            int j;
            (newThread[i] = new threadZad3(number, array)).start();

            for (j = number; j < size; j++) {
                if (array[j] > number) {
                    number = array[j];

                    if ((i + 1) == numThreads) {
                        i = -1;
                    }

                    if (newThread[i + 1] != null) {
                        try {
                            newThread[i + 1].join();
                        } catch (InterruptedException e) {
                            System.out.println(e.getMessage());
                        }
                    }
                    break;
                }
            }

            if (j == size) {
                break;
            }
        }

        for (int i = 0; i < numThreads; i++) {
            try {
                newThread[i].join();
            } catch (InterruptedException e) {
                System.out.println(e.getMessage());
            }
        }

        System.out.println("Liczby pierwsze w przedziale od 0 do " + size + ":");

        for (int i = 0, count = 0; i < size; i++) {
            if (array[i] != 0) {
                System.out.print(array[i] + " ");
                ++count;

                if (count % 30 == 0) {
                    System.out.println();
                }
            }
        }
    }
}
