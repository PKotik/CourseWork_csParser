public class MyClass {
    int x;
    private int y;
    static int z;

    int Method1() {
        int a = 5;
        int b;
        a = 10;
        if (a >= b) {
            return a;
        } else {
            return b;
        }
    }

    void Method2() {
        int x = 0;
        while (x <= 10) {
            x = x + 1;
        }
        return;
    }

    void Method3() {
        return 8;  // Ошибка: void не может возвращать значение
    }

    int Method4() {
        return;    // Ошибка: int должен возвращать значение
    }
}