package utfpr.model;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class Consulta {
    private String cep;
    private String dataHora;

    public Consulta(String cep) {
        this.cep = cep;
        this.dataHora = LocalDateTime.now().format(DateTimeFormatter.ofPattern("dd/MM/yyyy HH:mm:ss"));
    }

    public String getCep() {
        return cep;
    }

    public String getDataHora() {
        return dataHora;
    }

    @Override
    public String toString() {
        return "CEP: " + cep + " - " + dataHora;
    }
}
