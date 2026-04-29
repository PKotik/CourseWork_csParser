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
        return;  // Ошибка: void не может возвращать значение
    }

    int Method4() {
        return 7;    // Ошибка: int должен возвращать значение
    }
}
