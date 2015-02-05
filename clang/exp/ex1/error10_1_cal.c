void cal(int sel)
{
    double vol_ball();
    double vol_cylind();
    double vol_cone();
    switch (sel) {
        case 1:
        printf("球体体积为：%.2f\n", vol_ball());
        break;
        case 2:
        printf("圆柱体体积为：%.2f\n", vol_cylind());
        break;
        case 3:
        printf("圆锥体体积为：%.2f\n", vol_cone());
        break;
    }
}
