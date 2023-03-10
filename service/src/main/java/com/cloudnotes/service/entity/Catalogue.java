package com.cloudnotes.service.entity;

import lombok.Data;

import javax.persistence.*;

@Entity
@Data
@Table(name = "catalogue")
public class Catalogue {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;
    @Column(name = "name")
    private String name;
    @Column(name = "user_id")
    private int userId;
    @Column(name = "father_id")
    private int fatherId;
}
