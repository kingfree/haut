package exp6;

import java.awt.BorderLayout;
import java.awt.GridLayout;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;

public class LoginWindow extends JFrame {

    private static final long serialVersionUID = 1L;
    private JPanel 主面板 = null;
    private JLabel 用户标签 = null;
    private JLabel 密码标签 = null;
    private JTextField 用户名框 = null;
    private JButton 确认按钮 = null;
    private JButton 取消按钮 = null;
    private LoginWindow 主窗口 = null;
    private JPasswordField 密码框 = null;

    private static String username = "root";
    private static String password = "root";

    public LoginWindow() {
        super();
        初始化();
        主窗口 = this;
    }

    private void 初始化() {
        this.setTitle("登录");
        this.setContentPane(取主面板());
        pack();
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setLocationRelativeTo(null); // 居中
        this.setVisible(true);
    }

    private JPanel 取主面板() {
        if (主面板 == null) {
            主面板 = new JPanel();
            主面板.setLayout(new BorderLayout());
            用户标签 = new JLabel("用户名: ", SwingConstants.RIGHT);
            密码标签 = new JLabel("密码: ", SwingConstants.RIGHT);
            JPanel 输入面板 = new JPanel();
            输入面板.setLayout(new GridLayout(2, 2));
            输入面板.add(用户标签);
            输入面板.add(取用户名框());
            输入面板.add(密码标签);
            输入面板.add(取密码框());
            主面板.add(输入面板, BorderLayout.NORTH);
            JPanel 确认面板 = new JPanel();
            确认面板.add(取确认按钮());
            确认面板.add(取取消按钮());
            主面板.add(确认面板, BorderLayout.SOUTH);
        }
        return 主面板;
    }

    private JTextField 取用户名框() {
        if (用户名框 == null) {
            用户名框 = new JTextField();
            用户名框.setColumns(8);
        }
        return 用户名框;
    }

    private JPasswordField 取密码框() {
        if (密码框 == null) {
            密码框 = new JPasswordField();
            密码框.setColumns(8);
            密码框.setEchoChar('*');
        }
        return 密码框;
    }

    private JButton 取确认按钮() {
        if (确认按钮 == null) {
            确认按钮 = new JButton("确定");
            确认按钮.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(java.awt.event.ActionEvent e) {
                    String username = 主窗口.用户名框.getText().trim().toLowerCase();
                    String password = new String(主窗口.密码框.getPassword());
                    if (username.equals(LoginWindow.username)
                            && password.equals(LoginWindow.password)) {
                        // JOptionPane.showMessageDialog(null, "登录成功");
                        主窗口.setVisible(false);
                        SwingUtilities.invokeLater(new Runnable() {
                            public void run() {
                                new MainWindow();
                            }
                        });
                    } else {
                        JOptionPane.showMessageDialog(null, "登录失败");
                    }
                }
            });
        }
        return 确认按钮;
    }

    private JButton 取取消按钮() {
        if (取消按钮 == null) {
            取消按钮 = new JButton("取消");
            取消按钮.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(java.awt.event.ActionEvent e) {
                    主窗口.用户名框.setText("");
                    主窗口.密码框.setText("");
                }
            });
        }
        return 取消按钮;
    }

}
