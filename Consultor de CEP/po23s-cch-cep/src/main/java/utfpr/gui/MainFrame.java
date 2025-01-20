package utfpr.gui;

import utfpr.http.ClienteHttp;
import org.json.JSONObject;
import utfpr.model.Consulta;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;

public class MainFrame extends JFrame {
    private JTextField txtCep;
    private JTextArea resultArea;
    private ArrayList<Consulta> historico; // lista pra armazenar o histórico com data

    public MainFrame() {
        setTitle("Consulta CEP");
        setSize(500, 300);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null); // centraliza a janela na tela

        historico = new ArrayList<>(); // inicializa o histórico

        // painel principal
        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new BorderLayout(10, 10));
        mainPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        // painel superior
        JPanel inputPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 10));
        JLabel lblCep = new JLabel("CEP:");
        txtCep = new JTextField(20);
        inputPanel.add(lblCep);
        inputPanel.add(txtCep);

        // painel central
        JPanel buttonPanel = new JPanel(new GridLayout(1, 3, 10, 10));
        JButton btnConsultar = new JButton("Consultar");
        JButton btnLimpar = new JButton("Limpar");
        JButton btnHistorico = new JButton("Histórico");
        buttonPanel.add(btnConsultar);
        buttonPanel.add(btnLimpar);
        buttonPanel.add(btnHistorico);

        // painel pra exibir os resultados
        resultArea = new JTextArea();
        resultArea.setEditable(false);
        resultArea.setBorder(BorderFactory.createLineBorder(Color.GRAY));
        JScrollPane scrollPane = new JScrollPane(resultArea);

        // add os paineis ao painel principal
        mainPanel.add(inputPanel, BorderLayout.NORTH);
        mainPanel.add(scrollPane, BorderLayout.CENTER);
        mainPanel.add(buttonPanel, BorderLayout.SOUTH);

        // add ações aos botoes
        btnConsultar.addActionListener(e -> consultarCep());
        btnLimpar.addActionListener(e -> limparCampos());
        btnHistorico.addActionListener(e -> exibirHistorico());

        add(mainPanel);
        setVisible(true);
    }

    // construtores

    // faz a conexão com a api e insere os dados nas variaveis
    private void consultarCep() {
        String cep = txtCep.getText().trim();
        if (cep.isEmpty() || cep.length() != 8 || !cep.matches("\\d+")) {
            JOptionPane.showMessageDialog(this, "Por favor, insira um CEP válido (8 dígitos numéricos).", "Erro", JOptionPane.ERROR_MESSAGE);
            return;
        }

        try {
            ClienteHttp clienteHttp = new ClienteHttp();
            String url = "https://viacep.com.br/ws/" + cep + "/json/";
            String resposta = clienteHttp.buscaDados(url);

            JSONObject json = new JSONObject(resposta);
            if (json.has("erro")) {
                resultArea.setText("CEP não encontrado.");
            } else {
                String endereco = String.format(
                        "Logradouro: %s\nComplemento: %s\nBairro: %s\nCidade: %s\nEstado: %s\nDDD: %s\nCEP: %s",
                        json.optString("logradouro", "N/A"),
                        json.optString("complemento", "N/A"),
                        json.optString("bairro", "N/A"),
                        json.optString("localidade", "N/A"),
                        json.optString("uf", "N/A"),
                        json.optString("ddd", "N/A"),
                        json.optString("cep", "N/A")
                );
                resultArea.setText(endereco);

                // adiciona a consulta no histórico
                historico.add(new Consulta(cep));
            }
        } catch (Exception ex) {
            resultArea.setText("Erro ao consultar o CEP: " + ex.getMessage());
        }
    }

    private void limparCampos() {
        txtCep.setText("");
        resultArea.setText("");
    }

    private void exibirHistorico() {
        if (historico.isEmpty()) {
            JOptionPane.showMessageDialog(this, "O histórico está vazio.", "Histórico", JOptionPane.INFORMATION_MESSAGE);
            return;
        }

        // cria um painel pro histórico
        JDialog historicoDialog = new JDialog(this, "Histórico de Consultas", true);
        historicoDialog.setSize(300, 200);
        historicoDialog.setLocationRelativeTo(this);

        JTextArea historicoArea = new JTextArea();
        historicoArea.setEditable(false);
        for (Consulta consulta : historico) {
            historicoArea.append(consulta + "\n");
        }

        JScrollPane scrollPane = new JScrollPane(historicoArea);
        historicoDialog.add(scrollPane);

        // botãoo pra limpar o histórico
        JButton btnLimparHistorico = new JButton("Limpar Histórico");
        btnLimparHistorico.addActionListener(e -> {
            historico.clear();
            historicoDialog.dispose();
            JOptionPane.showMessageDialog(this, "Histórico limpo.", "Histórico", JOptionPane.INFORMATION_MESSAGE);
        });

        historicoDialog.add(btnLimparHistorico, BorderLayout.SOUTH);
        historicoDialog.setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(MainFrame::new);
    }
}
