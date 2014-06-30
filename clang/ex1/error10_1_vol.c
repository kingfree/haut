double vol_ball()
{
    double r;
    printf("请输入球体的半径：");
    scanf("%lf", &r);
    return 4.0 / 3.0 * PI * r * r * r;
}

double vol_cylind()
{
    double r, h;
    printf("请输入圆柱体的底面半径和高：");
    scanf("%lf%lf", &r, &h);
    return PI * r * r * h;
}

double vol_cone()
{
    double r, h;
    printf("请输入圆锥体的底面半径和高：");
    scanf("%lf%lf", &r, &h);
    return PI * r * r * h / 3.0;
}
