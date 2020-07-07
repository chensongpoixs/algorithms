

import java.util.Arrays;
 
/**
 * 
 * ��׼��ͼ��
 */
public class ctest_draw {

	


    //��ͼ����
    private static void Drawing() {
        /**
         * ��׼��ͼ��ľ�̬����
         *  Ĭ�ϵĻ����ǵ�λ������Ҳ����0-1
         * */
//      1������һ��ֱ�ߣ�A(0.3,0.2)��B(0,0)�������������ӳɵ�ֱ�ߣ���ΪĬ���ǵ�λ���������������������1�Ļ�����ʾ��������
//        StdDraw.line(0.3, 0.2, 0, 0);
 
//      2������һ��ͬ��Բ��Բ����(0.5,0.5)���뾶��0.2
//        StdDraw.circle(0.5, 0.5, 0.2);
 
//      3������һ�����������ĵ���(0.5,0.5),���ĵ������ߵ��ߵľ�����0.2
//        StdDraw.square(0.5, 0.5, 0.2);
//      4������һ�������β��������
//        StdDraw.filledSquare(0.5,0.5,0.2);
 
        /*
         * 5������һ�������
         * ������ֱ���(0.2,0.1),(0.3,0.6),(0.4,0.9)
         * ����֮�����ߣ��ɵõ�һ�������
         * */
        double[] x = {0.2, 0.3, 0.4};
        double[] y = {0.1, 0.6, 0.9};
//        StdDraw.polygon(x, y);
//      6����������ͣ���������䣬Ĭ���Ǻ�ɫ
//        StdDraw.filledPolygon(x, y);
 
//      7��������һ��һ�������
//        StdDraw.point(0.3,0.4);
        /*
         * 8������һ����Բ�Σ����ĵ�(0.2,0.3);
         * ˮƽ���ϵİ��᳤:0.4;
         * ��ֱ���ϵİ��᳤:0.2
         * */
//        StdDraw.ellipse(0.2, 0.3, 0.4, 0.2);
//        9��������Բ�Σ����������
//        StdDraw.filledEllipse(0.2, 0.3, 0.4, 0.2);
 
//        10������һ�����Σ��ֱ��Ӧ���ǣ����ĵ㡢���ȡ��볤��
//        StdDraw.rectangle(0.5,0.5,0.2,0.4);
 
//        11������һ�����Σ����������
//        StdDraw.filledRectangle(0.5,0.5,0.2,0.4);
 
    }
 
    //���Ʒ���
    //���磺�ı仭���Ĵ�С�ͱ�����ֱ�ߵ���ɫ�Ϳ�ȡ��ı�����ȵ�
    private static void control() {
        //��x����������Ϊ0-10
    	
    	StdDraw.setXscale(0, 10);
        //��y����������Ϊ0-10
        StdDraw.setYscale(0, 10);
 
        //�Ի��ʵĴ�ϸ�����趨
//        StdDraw.setPenRadius(0.01);
 
        //���û��ʵ���ɫ
        StdDraw.setPenColor(StdDraw.RED);
 
        //���û����Ŀ�͸�
//        StdDraw.setCanvasSize(200,300);
 
        //��ջ���������ɫC����������
//        StdDraw.clear(StdDraw.BLUE);
        StdDraw.line(3, 1, 9, 2);
 
        //��ʾ����ͼ�񣬲���ͣ**����
//        StdDraw.show();
 
    }
 
    /**
     * ��ͼ����
     */
    //����ֵ
    private static void def_value() {
 
        int N = 100;
        StdDraw.setXscale(0, N);
        StdDraw.setYscale(0, N * N);
 
        StdDraw.setPenRadius(.01);
        for (int i = 1; i <= N; i++) {
            StdDraw.point(i, i);
            StdDraw.point(i, i * Math.log(i));
            StdDraw.point(i, i * i);
        }
    }
 
    //�������
    private static void random_array() {
        int N = 50;
        double[] a = new double[N];
        //����õ�0-1֮���ʵ��,����������и�ֵ
        for (int i = 0; i < N; i++) a[i] = StdRandom.random();
 
        for (int i = 0; i < N; i++) {
            //����1.0Ϊ���ǽ����Ϊdouble����
            double x = 1.0 * i / N;
            double y = a[i] / 2.0;
 
            double width = 0.5 / N;
 
            double high = a[i] / 2.0;
            StdDraw.filledRectangle(x, y, width, high);
        }
    }
 
    //��������������
    private static void random_array_sort() {
        int N = 50;
        double[] a = new double[N];
        //����õ�0-1֮���ʵ��,����������и�ֵ
        for (int i = 0; i < N; i++) a[i] = StdRandom.random();
        //�������е�ֵ��������
        Arrays.sort(a);
        for (int i = 0; i < N; i++) {
            //����1.0Ϊ���ǽ����Ϊdouble����
            double x = 1.0 * i / N;
            double y = a[i] / 2.0;
 
            double width = 0.5 / N;
 
            double high = a[i] / 2.0;
            StdDraw.filledRectangle(x, y, width, high);
        }
    }
 
 
    public static void main(String[] args) {
        //��ͼ����
//        Drawing();
 
        //���Ʒ���
//        control();
 
        //����ֵ
//        def_value();
 
        //�������
//        random_array();
 
        //��������������
//        random_array_sort();
 
    }


}
